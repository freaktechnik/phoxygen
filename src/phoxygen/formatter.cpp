/*
 * phoxygen -- PHP documentation tool. (C) 2015--2016 Baubadil GmbH.
 *
 * phoxygen is free software; you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, in version 2 as it comes
 * in the "LICENSE" file of the phoxygen main distribution. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LICENSE file for more details.
 */

#include "xwp/basetypes.h"
#include "xwp/stringhelp.h"
#include "xwp/regex.h"

#include "phoxygen/formatter.h"

FormatterPlain g_fmtPlain;
FormatterHTML g_fmtHTML;
FormatterLatex g_fmtLatex;

/***************************************************************************
 *
 *  FormatterBase
 *
 **************************************************************************/

const string FormatterBase::Empty = "";
const string FormatterBase::TwoNewlines = "\n\n";
const string FormatterBase::TwoDashes = " --- ";

/* static */
FormatterBase& FormatterBase::Get(OutputMode mode)
{
    switch (mode)
    {
        case OutputMode::PLAINTEXT:
            return g_fmtPlain;

        case OutputMode::HTML:
            return g_fmtHTML;

        case OutputMode::LATEX:
        break;
    }

    // LaTeX
    return g_fmtLatex;
}

/***************************************************************************
 *
 *  FormatterHTML
 *
 **************************************************************************/

const string FormatterHTML::OpenP = "<p>";
const string FormatterHTML::CloseP = "</p>";
const string FormatterHTML::OpenPRE = "\n<pre>\n";
const string FormatterHTML::ClosePRE = "\n</pre>\n";
const string FormatterHTML::OpenUL = "<ul>";
const string FormatterHTML::CloseUL = "</ul>\n\n";
const string FormatterHTML::OpenOL = "<ol>";
const string FormatterHTML::CloseOL = "</ol>\n\n";
const string FormatterHTML::OpenLI = "<li>";
const string FormatterHTML::EndLI = "</li>";
const string FormatterHTML::OpenCODE = "<code>";
const string FormatterHTML::CloseCODE = "</code>";
const string FormatterHTML::MDash = " &mdash; ";

/* virtual */
string FormatterHTML::format(const string &str) /* override */
{
    return ::toHTML2(str);
}

/* virtual */
void FormatterHTML::convertFormatting(string &str) /* override */
{
    /* format() above converted <TAG> to &lt;TAG&gt; --
       permit the following HTML tags by converting &lt;TAG&gt; back to <TAG>: */
    static const StringVector svTags( { "ol", "ul", "li", "b", "i", "code" } );
    static const Regex reTags("&lt;(/?(?:" + implode("|", svTags) + "))&gt;");
    reTags.findReplace(str, "<$1>", true);

    static const Regex reCode("`([^`]+)`");
    reCode.findReplace(str, "<code>$1</code>", true);

    // Linkify links.
    static const Regex reLink("(https?://\\S+)");
            // "(?:^|[\\W])((ht|f)tp(s?):\\/\\/|www\\.)(([\\w\\-]+\\.){1,}?([\\w\\-.~]+\\/?)*[\\[a-zA-Z0-9].,%_=?&#\\-+()\\[\\]\\*$~@!:/{};']*)");
    reLink.findReplace(str, "<a href=\"$1\">$1</a>", true);
}

/* virtual */
string FormatterHTML::makeLink(const string &strIdentifier,     //!< in: including "table_" etc. prefix
                               const string *pstrAnchor,
                               const string &strTitle) /* override */
{
    string str = "<a href=\"" + strIdentifier + ".html";
    if (pstrAnchor)
        str += "#" + *pstrAnchor;
    return str + "\">" + strTitle + "</a>";
}

/* virtual */
string FormatterHTML::makeHeading(uint level, const string &str) /* override */
{
    string strLevel = to_string(level);
    return "\n<h" + strLevel + ">" + str + "</h" + strLevel + ">\n";
}


/***************************************************************************
 *
 *  FormatterLaTeX
 *
 **************************************************************************/

const string FormatterLatex::BeginVerbatim = "\n\\begin{alltt}\n";
const string FormatterLatex::EndVerbatim = "\n\\end{alltt}\n";
const string FormatterLatex::BeginItemize = "\\begin{itemize}\n";
const string FormatterLatex::EndItemize = "\n\\end{itemize}";
const string FormatterLatex::BeginEnumerate = "\\begin{enumerate}\n";
const string FormatterLatex::EndEnumerate = "\n\\end{enumerate}";
const string FormatterLatex::Item = "\\item ";
const string FormatterLatex::OpenTextTT = "\\texttt{";
const string FormatterLatex::CloseCurly = "}";

/* virtual */
string FormatterLatex::makeTarget(const string &prefix,
                                  const string &identifier) /* override */
{
    string str = identifier;
    stringReplace(str, "_", "@");
    return prefix + "@" + str;
}

/* virtual */
string FormatterLatex::format(const string &str) /* override */
{
    return ::toLaTeX2(str);
}

/* virtual */
void FormatterLatex::convertFormatting(string &str) /* override */
{
    static const Regex re2(R"i____(<ul>)i____");
    re2.findReplace(str, BeginItemize, true);
    static const Regex re3(R"i____(<\/ul>)i____");
    re3.findReplace(str, EndItemize, true);
    static const Regex re4(R"i____(<ol>)i____");
    re4.findReplace(str, BeginEnumerate, true);
    static const Regex re5(R"i____(<\/ol>)i____");
    re5.findReplace(str, EndEnumerate, true);
    static const Regex re6(R"i____(<li>)i____");
    re6.findReplace(str, Item, true);
    static const Regex re7(R"i____(<\/li>)i____");
    re7.findReplace(str, Empty, true);

    static const Regex re12(R"i____(<b>)i____");
    re12.findReplace(str, "\\textbf{", true);
    static const Regex re13(R"i____(<i>)i____");
    re13.findReplace(str, "\\textit{", true);
    static const Regex re14(R"i____(<\/[bi]>)i____");
    re14.findReplace(str, "}", true);

    // Markdown
    static const Regex reCode("`([^`]+)`");
    reCode.findReplace(str, OpenTextTT + "$1" + CloseCurly, true);

    // Linkify links
    static const Regex reLink("(https?://\\S+)");
            // "(?:^|[\\W])((ht|f)tp(s?):\\/\\/|www\\.)(([\\w\\-]+\\.){1,}?([\\w\\-.~]+\\/?)*[\\[a-zA-Z0-9].,%_=?&#\\-+()\\[\\]\\*$~@!:/{};']*)");
    reLink.findReplace(str, "\\url{$1}", true);
}

/* virtual */
string FormatterLatex::makeLink(const string &strIdentifier,
                                const string *pstrAnchor,
                                const string &strTitle)
{
    string str = strIdentifier;
    stringReplace(str, "_", "@");
    return "\\hyperref[" + str + "]{" + strTitle + "}";
}

/* virtual */
string FormatterLatex::makeHeading(uint level, const string &str) /* override */
{
    string str0;
    switch (level)
    {
        case 1:
            str0 = "\\section{";
        break;

        case 2:
            str0 = "\\subsection{";
        break;

        default:
            str0 = "\\subsubsection{";
        break;
    }

    return str0 + format(str) + "}";
}
