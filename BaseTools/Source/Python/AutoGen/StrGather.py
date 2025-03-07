## @file
# This file is used to parse a strings file and create or add to a string database
# file.
#
# Copyright (c) 2007 - 2018, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent

##
# Import Modules
#
from __future__ import absolute_import
import re
import Common.EdkLogger as EdkLogger
from Common.BuildToolError import *
from .UniClassObject import *
from io import BytesIO
from struct import pack, unpack
from Common.LongFilePathSupport import OpenLongFilePath as open

##
# Static definitions
#
EFI_HII_SIBT_END = '0x00'
EFI_HII_SIBT_STRING_SCSU = '0x10'
EFI_HII_SIBT_STRING_SCSU_FONT = '0x11'
EFI_HII_SIBT_STRINGS_SCSU = '0x12'
EFI_HII_SIBT_STRINGS_SCSU_FONT = '0x13'
EFI_HII_SIBT_STRING_UCS2 = '0x14'
EFI_HII_SIBT_STRING_UCS2_FONT = '0x15'
EFI_HII_SIBT_STRINGS_UCS2 = '0x16'
EFI_HII_SIBT_STRINGS_UCS2_FONT = '0x17'
EFI_HII_SIBT_DUPLICATE = '0x20'
EFI_HII_SIBT_SKIP2 = '0x21'
EFI_HII_SIBT_SKIP1 = '0x22'
EFI_HII_SIBT_EXT1 = '0x30'
EFI_HII_SIBT_EXT2 = '0x31'
EFI_HII_SIBT_EXT4 = '0x32'
EFI_HII_SIBT_FONT = '0x40'

EFI_HII_PACKAGE_STRINGS = '0x04'
EFI_HII_PACKAGE_FORM = '0x02'

StringPackageType = EFI_HII_PACKAGE_STRINGS
StringPackageForm = EFI_HII_PACKAGE_FORM
StringBlockType = EFI_HII_SIBT_STRING_UCS2
StringSkipType = EFI_HII_SIBT_SKIP2

HexHeader = '0x'

COMMENT = '// '
DEFINE_STR = '#define'
COMMENT_DEFINE_STR = COMMENT + DEFINE_STR
NOT_REFERENCED = 'not referenced'
COMMENT_NOT_REFERENCED = ' ' + COMMENT + NOT_REFERENCED
CHAR_ARRAY_DEFIN = 'unsigned char'
COMMON_FILE_NAME = 'Strings'
STRING_TOKEN = re.compile('STRING_TOKEN *\(([A-Z0-9_]+) *\)', re.MULTILINE | re.UNICODE)

EFI_HII_ARRAY_SIZE_LENGTH = 4
EFI_HII_PACKAGE_HEADER_LENGTH = 4
EFI_HII_HDR_SIZE_LENGTH = 4
EFI_HII_STRING_OFFSET_LENGTH = 4
EFI_STRING_ID = 1
EFI_STRING_ID_LENGTH = 2
EFI_HII_LANGUAGE_WINDOW = 0
EFI_HII_LANGUAGE_WINDOW_LENGTH = 2
EFI_HII_LANGUAGE_WINDOW_NUMBER = 16
EFI_HII_STRING_PACKAGE_HDR_LENGTH = EFI_HII_PACKAGE_HEADER_LENGTH + EFI_HII_HDR_SIZE_LENGTH + EFI_HII_STRING_OFFSET_LENGTH + EFI_HII_LANGUAGE_WINDOW_LENGTH * EFI_HII_LANGUAGE_WINDOW_NUMBER + EFI_STRING_ID_LENGTH

H_C_FILE_HEADER = ['//', \
                   '//  DO NOT EDIT -- auto-generated file', \
                   '//', \
                   '//  This file is generated by the StrGather utility', \
                   '//']
LANGUAGE_NAME_STRING_NAME = '$LANGUAGE_NAME'
PRINTABLE_LANGUAGE_NAME_STRING_NAME = '$PRINTABLE_LANGUAGE_NAME'

## Convert a dec number to a hex string
#
# Convert a dec number to a formatted hex string in length digit
# The digit is set to default 8
# The hex string starts with "0x"
# DecToHexStr(1000) is '0x000003E8'
# DecToHexStr(1000, 6) is '0x0003E8'
#
# @param Dec:    The number in dec format
# @param Digit:  The needed digit of hex string
#
# @retval:       The formatted hex string
#
def DecToHexStr(Dec, Digit = 8):
    return '0x{0:0{1}X}'.format(Dec, Digit)

## Convert a dec number to a hex list
#
# Convert a dec number to a formatted hex list in size digit
# The digit is set to default 8
# DecToHexList(1000) is ['0xE8', '0x03', '0x00', '0x00']
# DecToHexList(1000, 6) is ['0xE8', '0x03', '0x00']
#
# @param Dec:    The number in dec format
# @param Digit:  The needed digit of hex list
#
# @retval:       A list for formatted hex string
#
def DecToHexList(Dec, Digit = 8):
    Hex = '{0:0{1}X}'.format(Dec, Digit)
    return ["0x" + Hex[Bit:Bit + 2] for Bit in range(Digit - 2, -1, -2)]

## Convert a acsii string to a hex list
#
# Convert a acsii string to a formatted hex list
# AscToHexList('en-US') is ['0x65', '0x6E', '0x2D', '0x55', '0x53']
#
# @param Ascii:  The acsii string
#
# @retval:       A list for formatted hex string
#
def AscToHexList(Ascii):
    try:
        return ['0x{0:02X}'.format(Item) for Item in Ascii]
    except:
        return ['0x{0:02X}'.format(ord(Item)) for Item in Ascii]

## Create content of .h file
#
# Create content of .h file
#
# @param BaseName:        The basename of strings
# @param UniObjectClass   A UniObjectClass instance
# @param IsCompatibleMode Compatible mode
# @param UniGenCFlag      UniString is generated into AutoGen C file when it is set to True
#
# @retval Str:           A string of .h file content
#
def CreateHFileContent(BaseName, UniObjectClass, IsCompatibleMode, UniGenCFlag):
    Str = []
    ValueStartPtr = 60
    Line = COMMENT_DEFINE_STR + ' ' + LANGUAGE_NAME_STRING_NAME + ' ' * (ValueStartPtr - len(DEFINE_STR + LANGUAGE_NAME_STRING_NAME)) + DecToHexStr(0, 4) + COMMENT_NOT_REFERENCED
    Str = WriteLine(Str, Line)
    Line = COMMENT_DEFINE_STR + ' ' + PRINTABLE_LANGUAGE_NAME_STRING_NAME + ' ' * (ValueStartPtr - len(DEFINE_STR + PRINTABLE_LANGUAGE_NAME_STRING_NAME)) + DecToHexStr(1, 4) + COMMENT_NOT_REFERENCED
    Str = WriteLine(Str, Line)
    UnusedStr = ''

    #Group the referred/Unused STRING token together.
    for Index in range(2, len(UniObjectClass.OrderedStringList[UniObjectClass.LanguageDef[0][0]])):
        StringItem = UniObjectClass.OrderedStringList[UniObjectClass.LanguageDef[0][0]][Index]
        Name = StringItem.StringName
        Token = StringItem.Token
        Referenced = StringItem.Referenced
        if Name is not None:
            Line = ''
            if Referenced == True:
                if (ValueStartPtr - len(DEFINE_STR + Name)) <= 0:
                    Line = DEFINE_STR + ' ' + Name + ' ' + DecToHexStr(Token, 4)
                else:
                    Line = DEFINE_STR + ' ' + Name + ' ' * (ValueStartPtr - len(DEFINE_STR + Name)) + DecToHexStr(Token, 4)
                Str = WriteLine(Str, Line)
            else:
                if (ValueStartPtr - len(DEFINE_STR + Name)) <= 0:
                    Line = COMMENT_DEFINE_STR + ' ' + Name + ' ' + DecToHexStr(Token, 4) + COMMENT_NOT_REFERENCED
                else:
                    Line = COMMENT_DEFINE_STR + ' ' + Name + ' ' * (ValueStartPtr - len(DEFINE_STR + Name)) + DecToHexStr(Token, 4) + COMMENT_NOT_REFERENCED
                UnusedStr = WriteLine(UnusedStr, Line)

    Str.extend( UnusedStr)

    Str = WriteLine(Str, '')
    if IsCompatibleMode or UniGenCFlag:
        Str = WriteLine(Str, 'extern unsigned char ' + BaseName + 'Strings[];')
    return "".join(Str)

## Create a complete .h file
#
# Create a complet .h file with file header and file content
#
# @param BaseName:        The basename of strings
# @param UniObjectClass   A UniObjectClass instance
# @param IsCompatibleMode Compatible mode
# @param UniGenCFlag      UniString is generated into AutoGen C file when it is set to True
#
# @retval Str:           A string of complete .h file
#
def CreateHFile(BaseName, UniObjectClass, IsCompatibleMode, UniGenCFlag):
    HFile = WriteLine('', CreateHFileContent(BaseName, UniObjectClass, IsCompatibleMode, UniGenCFlag))

    return "".join(HFile)

## Create a buffer to store all items in an array
#
# @param BinBuffer   Buffer to contain Binary data.
# @param Array:      The array need to be formatted
#
def CreateBinBuffer(BinBuffer, Array):
    for Item in Array:
        BinBuffer.write(pack("B", int(Item, 16)))

## Create a formatted string all items in an array
#
# Use ',' to join each item in an array, and break an new line when reaching the width (default is 16)
#
# @param Array:      The array need to be formatted
# @param Width:      The line length, the default value is set to 16
#
# @retval ArrayItem: A string for all formatted array items
#
def CreateArrayItem(Array, Width = 16):
    MaxLength = Width
    Index = 0
    Line = '  '
    ArrayItem = []

    for Item in Array:
        if Index < MaxLength:
            Line = Line + Item + ',  '
            Index = Index + 1
        else:
            ArrayItem = WriteLine(ArrayItem, Line)
            Line = '  ' + Item + ',  '
            Index = 1
    ArrayItem = Write(ArrayItem, Line.rstrip())

    return "".join(ArrayItem)

## CreateCFileStringValue
#
# Create a line with string value
#
# @param Value:  Value of the string
#
# @retval Str:   A formatted string with string value
#

def CreateCFileStringValue(Value):
    Value = [StringBlockType] + Value
    Str = WriteLine('', CreateArrayItem(Value))

    return "".join(Str)

## GetFilteredLanguage
#
# apply get best language rules to the UNI language code list
#
# @param UniLanguageList:  language code definition list in *.UNI file
# @param LanguageFilterList:  language code filter list of RFC4646 format in DSC file
#
# @retval UniLanguageListFiltered:   the filtered language code
#
def GetFilteredLanguage(UniLanguageList, LanguageFilterList):
    UniLanguageListFiltered = []
    # if filter list is empty, then consider there is no filter
    if LanguageFilterList == []:
        UniLanguageListFiltered = UniLanguageList
        return UniLanguageListFiltered
    for Language in LanguageFilterList:
        # first check for exact match
        if Language in UniLanguageList:
            if Language not in UniLanguageListFiltered:
                UniLanguageListFiltered.append(Language)
        # find the first one with the same/equivalent primary tag
        else:
            if Language.find('-') != -1:
                PrimaryTag = Language[0:Language.find('-')].lower()
            else:
                PrimaryTag = Language

            if len(PrimaryTag) == 3:
                PrimaryTag = LangConvTable.get(PrimaryTag)

            for UniLanguage in UniLanguageList:
                if UniLanguage.find('-') != -1:
                    UniLanguagePrimaryTag = UniLanguage[0:UniLanguage.find('-')].lower()
                else:
                    UniLanguagePrimaryTag = UniLanguage

                if len(UniLanguagePrimaryTag) == 3:
                    UniLanguagePrimaryTag = LangConvTable.get(UniLanguagePrimaryTag)

                if PrimaryTag == UniLanguagePrimaryTag:
                    if UniLanguage not in UniLanguageListFiltered:
                        UniLanguageListFiltered.append(UniLanguage)
                    break
            else:
                # Here is rule 3 for "get best language"
                # If tag is not listed in the Unicode file, the default ("en") tag should be used for that language
                # for better processing, find the one that best suit for it.
                DefaultTag = 'en'
                if DefaultTag not in UniLanguageListFiltered:
                    # check whether language code with primary code equivalent with DefaultTag already in the list, if so, use that
                    for UniLanguage in UniLanguageList:
                        if UniLanguage.startswith('en-') or UniLanguage.startswith('eng-'):
                            if UniLanguage not in UniLanguageListFiltered:
                                UniLanguageListFiltered.append(UniLanguage)
                            break
                    else:
                        UniLanguageListFiltered.append(DefaultTag)
    return  UniLanguageListFiltered


## Create content of .c file
#
# Create content of .c file
#
# @param BaseName:        The basename of strings
# @param UniObjectClass   A UniObjectClass instance
# @param IsCompatibleMode Compatible mode
# @param UniBinBuffer     UniBinBuffer to contain UniBinary data.
# @param FilterInfo       Platform language filter information
#
# @retval Str:           A string of .c file content
#
def CreateCFileContent(BaseName, UniObjectClass, IsCompatibleMode, UniBinBuffer, FilterInfo):
    #
    # Init array length
    #
    TotalLength = EFI_HII_ARRAY_SIZE_LENGTH
    Str = ''
    Offset = 0

    EDK2Module = FilterInfo[0]
    if EDK2Module:
        LanguageFilterList = FilterInfo[1]
    else:
        # EDK module is using ISO639-2 format filter, convert to the RFC4646 format
        LanguageFilterList = [LangConvTable.get(F.lower()) for F in FilterInfo[1]]

    UniLanguageList = []
    for IndexI in range(len(UniObjectClass.LanguageDef)):
        UniLanguageList += [UniObjectClass.LanguageDef[IndexI][0]]

    UniLanguageListFiltered = GetFilteredLanguage(UniLanguageList, LanguageFilterList)


    #
    # Create lines for each language's strings
    #
    for IndexI in range(len(UniObjectClass.LanguageDef)):
        Language = UniObjectClass.LanguageDef[IndexI][0]
        if Language not in UniLanguageListFiltered:
            continue

        StringBuffer = BytesIO()
        StrStringValue = ''
        ArrayLength = 0
        NumberOfUseOtherLangDef = 0
        Index = 0
        for IndexJ in range(1, len(UniObjectClass.OrderedStringList[UniObjectClass.LanguageDef[IndexI][0]])):
            Item = UniObjectClass.OrderedStringListByToken[Language][IndexJ]

            Name = Item.StringName
            Value = Item.StringValueByteList
            Referenced = Item.Referenced
            Token = Item.Token
            UseOtherLangDef = Item.UseOtherLangDef

            if UseOtherLangDef != '' and Referenced:
                NumberOfUseOtherLangDef = NumberOfUseOtherLangDef + 1
                Index = Index + 1
            else:
                if NumberOfUseOtherLangDef > 0:
                    StrStringValue = WriteLine(StrStringValue, CreateArrayItem([StringSkipType] + DecToHexList(NumberOfUseOtherLangDef, 4)))
                    CreateBinBuffer (StringBuffer, ([StringSkipType] + DecToHexList(NumberOfUseOtherLangDef, 4)))
                    NumberOfUseOtherLangDef = 0
                    ArrayLength = ArrayLength + 3
                if Referenced and Item.Token > 0:
                    Index = Index + 1
                    StrStringValue = WriteLine(StrStringValue, "// %s: %s:%s" % (DecToHexStr(Index, 4), Name, DecToHexStr(Token, 4)))
                    StrStringValue = Write(StrStringValue, CreateCFileStringValue(Value))
                    CreateBinBuffer (StringBuffer, [StringBlockType] + Value)
                    ArrayLength = ArrayLength + Item.Length + 1 # 1 is for the length of string type

        #
        # EFI_HII_PACKAGE_HEADER
        #
        Offset = EFI_HII_STRING_PACKAGE_HDR_LENGTH + len(Language) + 1
        ArrayLength = Offset + ArrayLength + 1

        #
        # Create PACKAGE HEADER
        #
        Str = WriteLine(Str, '// PACKAGE HEADER\n')
        TotalLength = TotalLength + ArrayLength

        List = DecToHexList(ArrayLength, 6) + \
               [StringPackageType] + \
               DecToHexList(Offset) + \
               DecToHexList(Offset) + \
               DecToHexList(EFI_HII_LANGUAGE_WINDOW, EFI_HII_LANGUAGE_WINDOW_LENGTH * 2) * EFI_HII_LANGUAGE_WINDOW_NUMBER + \
               DecToHexList(EFI_STRING_ID, 4) + \
               AscToHexList(Language) + \
               DecToHexList(0, 2)
        Str = WriteLine(Str, CreateArrayItem(List, 16) + '\n')

        #
        # Create PACKAGE DATA
        #
        Str = WriteLine(Str, '// PACKAGE DATA\n')
        Str = Write(Str, StrStringValue)

        #
        # Add an EFI_HII_SIBT_END at last
        #
        Str = WriteLine(Str, '  ' + EFI_HII_SIBT_END + ",")

        #
        # Create binary UNI string
        #
        if UniBinBuffer:
            CreateBinBuffer (UniBinBuffer, List)
            UniBinBuffer.write (StringBuffer.getvalue())
            UniBinBuffer.write (pack("B", int(EFI_HII_SIBT_END, 16)))
        StringBuffer.close()

    #
    # Create line for string variable name
    # "unsigned char $(BaseName)Strings[] = {"
    #
    AllStr = WriteLine('', CHAR_ARRAY_DEFIN + ' ' + BaseName + COMMON_FILE_NAME + '[] = {\n')

    if IsCompatibleMode:
        #
        # Create FRAMEWORK_EFI_HII_PACK_HEADER in compatible mode
        #
        AllStr = WriteLine(AllStr, '// FRAMEWORK PACKAGE HEADER Length')
        AllStr = WriteLine(AllStr, CreateArrayItem(DecToHexList(TotalLength + 2)) + '\n')
        AllStr = WriteLine(AllStr, '// FRAMEWORK PACKAGE HEADER Type')
        AllStr = WriteLine(AllStr, CreateArrayItem(DecToHexList(2, 4)) + '\n')
    else:
        #
        # Create whole array length in UEFI mode
        #
        AllStr = WriteLine(AllStr, '// STRGATHER_OUTPUT_HEADER')
        AllStr = WriteLine(AllStr, CreateArrayItem(DecToHexList(TotalLength)) + '\n')

    #
    # Join package data
    #
    AllStr = Write(AllStr, Str)

    return "".join(AllStr)

## Create end of .c file
#
# Create end of .c file
#
# @retval Str:           A string of .h file end
#
def CreateCFileEnd():
    Str = Write('', '};')
    return Str

## Create a .c file
#
# Create a complete .c file
#
# @param BaseName:        The basename of strings
# @param UniObjectClass   A UniObjectClass instance
# @param IsCompatibleMode Compatible Mode
# @param FilterInfo       Platform language filter information
#
# @retval CFile:          A string of complete .c file
#
def CreateCFile(BaseName, UniObjectClass, IsCompatibleMode, FilterInfo):
    CFile = ''
    CFile = WriteLine(CFile, CreateCFileContent(BaseName, UniObjectClass, IsCompatibleMode, None, FilterInfo))
    CFile = WriteLine(CFile, CreateCFileEnd())
    return "".join(CFile)

## GetFileList
#
# Get a list for all files
#
# @param IncludeList:  A list of all path to be searched
# @param SkipList:     A list of all types of file could be skipped
#
# @retval FileList:    A list of all files found
#
def GetFileList(SourceFileList, IncludeList, SkipList):
    if IncludeList is None:
        EdkLogger.error("UnicodeStringGather", AUTOGEN_ERROR, "Include path for unicode file is not defined")

    FileList = []
    if SkipList is None:
        SkipList = []

    for File in SourceFileList:
        for Dir in IncludeList:
            if not os.path.exists(Dir):
                continue
            File = os.path.join(Dir, File.Path)
            #
            # Ignore Dir
            #
            if os.path.isfile(File) != True:
                continue
            #
            # Ignore file listed in skip list
            #
            IsSkip = False
            for Skip in SkipList:
                if os.path.splitext(File)[1].upper() == Skip.upper():
                    EdkLogger.verbose("Skipped %s for string token uses search" % File)
                    IsSkip = True
                    break

            if not IsSkip:
                FileList.append(File)

            break

    return FileList

## SearchString
#
# Search whether all string defined in UniObjectClass are referenced
# All string used should be set to Referenced
#
# @param UniObjectClass:  Input UniObjectClass
# @param FileList:        Search path list
# @param IsCompatibleMode Compatible Mode
#
# @retval UniObjectClass: UniObjectClass after searched
#
def SearchString(UniObjectClass, FileList, IsCompatibleMode):
    if FileList == []:
        return UniObjectClass
    i = 0
    print("filename:")
    for File in FileList:
        i = 0
        Line = ""
        #print("filename: " + File)
        try:
            if os.path.isfile(File):
                Lines = open(File, 'r')
                for Line in Lines:
                    i += 1
                    #print("Line:" + Line)
                    for StrName in STRING_TOKEN.findall(Line):
                        #print("StrName: " + StrName)
                        EdkLogger.debug(EdkLogger.DEBUG_5, "Found string identifier: " + StrName)
                        UniObjectClass.SetStringReferenced(StrName)
        except:
            print("error line: %d", i)
            print("error line: %s", Line)
            EdkLogger.error("UnicodeStringGather", AUTOGEN_ERROR, "SearchString: Error while processing file", File=File, RaiseError=False)
            raise

    UniObjectClass.ReToken()

    return UniObjectClass

## GetStringFiles
#
# This function is used for UEFI2.1 spec
#
#
def GetStringFiles(UniFilList, SourceFileList, IncludeList, IncludePathList, SkipList, BaseName, IsCompatibleMode = False, ShellMode = False, UniGenCFlag = True, UniGenBinBuffer = None, FilterInfo = [True, []]):
    if len(UniFilList) > 0:
        if ShellMode:
            #
            # support ISO 639-2 codes in .UNI files of EDK Shell
            #
            Uni = UniFileClassObject(sorted(UniFilList, key=lambda x: x.File), True, IncludePathList)
        else:
            Uni = UniFileClassObject(sorted(UniFilList, key=lambda x: x.File), IsCompatibleMode, IncludePathList)
    else:
        EdkLogger.error("UnicodeStringGather", AUTOGEN_ERROR, 'No unicode files given')

    FileList = GetFileList(SourceFileList, IncludeList, SkipList)

    Uni = SearchString(Uni, sorted (FileList), IsCompatibleMode)

    HFile = CreateHFile(BaseName, Uni, IsCompatibleMode, UniGenCFlag)
    CFile = None
    if IsCompatibleMode or UniGenCFlag:
        CFile = CreateCFile(BaseName, Uni, IsCompatibleMode, FilterInfo)
    if UniGenBinBuffer:
        CreateCFileContent(BaseName, Uni, IsCompatibleMode, UniGenBinBuffer, FilterInfo)

    return HFile, CFile

#
# Write an item
#
def Write(Target, Item):
    if isinstance(Target,str):
        Target = [Target]
    if not Target:
        Target = []
    if isinstance(Item,list):
        Target.extend(Item)
    else:
        Target.append(Item)
    return Target

#
# Write an item with a break line
#
def WriteLine(Target, Item):
    if isinstance(Target,str):
        Target = [Target]
    if not Target:
        Target = []
    if isinstance(Item, list):
        Target.extend(Item)
    else:
        Target.append(Item)
    Target.append('\n')
    return Target

# This acts like the main() function for the script, unless it is 'import'ed into another
# script.
if __name__ == '__main__':
    EdkLogger.info('start')

    UniFileList = [
                   r'C:\\Edk\\Strings2.uni',
                   r'C:\\Edk\\Strings.uni'
    ]

    SrcFileList = []
    for Root, Dirs, Files in os.walk('C:\\Edk'):
        for File in Files:
            SrcFileList.append(File)

    IncludeList = [
                   r'C:\\Edk'
    ]

    SkipList = ['.inf', '.uni']
    BaseName = 'DriverSample'
    (h, c) = GetStringFiles(UniFileList, SrcFileList, IncludeList, SkipList, BaseName, True)
    hfile = open('unistring.h', 'w')
    cfile = open('unistring.c', 'w')
    hfile.write(h)
    cfile.write(c)

    EdkLogger.info('end')
