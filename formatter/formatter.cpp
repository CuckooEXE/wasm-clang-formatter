#include <clang/Format/Format.h>
#include <clang/Tooling/Core/Replacement.h>
#include <string.h>

#include <string>
#include <iostream>

using namespace clang;
using namespace clang::format;

__attribute__((export_name("malloc")))
void* _malloc(size_t n) {
    return malloc(n);
}

__attribute__((export_name("free")))
void _free(void* p) {
    free(p);
}

/**
 * @brief All the styles supported by clang-format
 */
static const char* Styles[] = {
    "None (Default)",
    "LLVM",       
    "Google",     
    "Chromium",   
    "Microsoft",  
    "GNU",        
    "WebKit",     
    "Mozilla",    
    "ClangFormat",
};

/**
 * @brief Given the name of the style, get the Format for it
 * @param styleName name of the style to retrieve
 * @param lang Language - defaults to CPP
 * @returns FormatStyle
 */
static FormatStyle getFormatStyle
(
    const std::string& styleName,
    FormatStyle::LanguageKind lang = FormatStyle::LanguageKind::LK_Cpp
)
{
    if (styleName == "LLVM")
        return getLLVMStyle(lang);

    if (styleName == "Google")
        return getGoogleStyle(lang);

    if (styleName == "Chromium")
        return getChromiumStyle(lang);

    if (styleName == "Microsoft")
        return getMicrosoftStyle(lang);

    if (styleName == "GNU")
        return getGNUStyle();

    if (styleName == "WebKit")
        return getWebKitStyle();

    if (styleName == "Mozilla")
        return getMozillaStyle();

    return getNoStyle();
}

/**
 * @brief Get the default style for a particular preset
 * @param ptr Pointer to memory containing the preset, and where we will write the output to
 * @param len Length of the pointer
 * @returns Number of bytes written (including null-terminator). -1 if the `ptr` wasn't long enough or if the preset wasn't found
 */
__attribute__((export_name("getStyle")))
int getStyle(char* ptr, int len) {
    if (!ptr || len <= 0)
        return -1;

    for (int i = 0; i < sizeof(Styles) / sizeof(Styles[0]); i++) {
        if (strncasecmp(Styles[i], ptr, strlen(Styles[i])) == 0) {
            std::string s = configurationAsText(getFormatStyle(Styles[i]));
            char* style = s.data();
            strcpy(ptr, style);
            ptr[strlen(style)] = '\0';
            return strlen(style) + 1;
        }
    }

    return -1;
}

/**
 * @brief Get a comma-delimited list of styles
 * @param ptr Pointer to memory to write the list to
 * @param len Length of the pointer
 * @returns Number of bytes written (including null-terminator). -1 if the `ptr` wasn't long enough
 */
__attribute__((export_name("getStyles")))
int getStyles(char* ptr, int len) {
    if (!ptr || len <= 0)
        return -1;

    int pos = 0;
    constexpr int count = sizeof(Styles) / sizeof(Styles[0]);

    for (int i = 0; i < count; ++i) {
        const char* s = Styles[i];

        int needed = strlen(s);
        if (i + 1 < count)
            needed += 1; // comma

        if (pos + needed >= len)
            return -1;

        memcpy(ptr + pos, s, strlen(s));
        pos += strlen(s);

        if (i + 1 < count)
            ptr[pos++] = ',';
    }

    if (pos >= len)
        return -1;

    ptr[pos++] = '\0';
    return pos;
}

/**
 * @brief Get the default style for a particular preset
 * @param configPtr Pointer to memory containing the configuration
 * @param configLen Length of the pointer
 * @param codePtr Pointer to memory containing the code, and where we will write the output to
 * @param codeLen Length of the pointer
 * @returns Number of bytes written (including null-terminator). -1 if the `ptr` wasn't long enough
 */
__attribute__((export_name("formatCode")))
int formatCode(char* configPtr, int configLen, char* codePtr, int codeLen) {
    if (!configPtr || configLen <= 0 || !codePtr || codeLen <= 0)
        return -1;

    FormatStyle myStyle;

    myStyle.Language = guessLanguage("", codePtr);
    if (myStyle.Language == FormatStyle::LanguageKind::LK_None)
        return -1;

    std::error_code e = parseConfiguration(configPtr, &myStyle);
    if (e)
        return -1;

    std::string code = codePtr;
    tooling::Replacements repls = reformat(myStyle, code, {tooling::Range(0, code.size())});
    llvm::Expected<std::string> formatted = tooling::applyAllReplacements(code, repls);
    if (!formatted)
        return -1;

    codePtr[(*formatted).size()] = '\0';
    strcpy(codePtr, (*formatted).data());
    return strlen(codePtr) + 1;
}
