/*
 *
 * Copyright 2013, Alexandre Laurent <shaoner>
 *
 * This file is part of spe4k.
 *
 * spe4k is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spe4k is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spe4k. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*!
 * \file text-formatter.hh
 * \author shaoner
 * \brief IRC text utils
 */
#ifndef TEXT_FORMATTER_HH
# define TEXT_FORMATTER_HH

# include "parameters.hh"

// Irc codes
# define CODE_IRC_ESCAPE '\x01'
# define CODE_IRC_COLOR '\x03'
# define CODE_IRC_BOLD '\x02'
# define CODE_IRC_ITALIC '\x1d'
# define CODE_IRC_RESET '\x0f'
# define CODE_IRC_STRIKETHROUGH '\x13'
# define CODE_IRC_REVERSE '\x16'
# define CODE_IRC_UNDERLINE '\x1f'

// Internal codes
# define CODE_FMT_COLOR "%C"
# define CODE_FMT_BOLD "%B"
# define CODE_FMT_ITALIC "%I"
# define CODE_FMT_RESET "%O"
# define CODE_FMT_STRIKETHROUGH "%S"
# define CODE_FMT_REVERSE "%R"
# define CODE_FMT_UNDERLINE "%U"

/*!
 * \class TextFormatter
 * \brief Text formatter for IRC
 */
class TextFormatter
{
public:
	static QString html_escape(const QString& text);
	static QString irc_to_html(const QString& raw);
private:
	static bool escape(QChar c, QString& text);
	static void set_html_tag(QString& text, bool& isOpened, const QString& openTag, const QString& closeTag);
	static void get_irc_color(int& color, bool& isColor, const QString& text, int& i);
	static void set_irc_color(int fg, int bg, bool opened, QString& text);
private:
};

inline bool
TextFormatter::escape(QChar c, QString& text)
{
	if (c == '&')
		text += "&amp;";
	else if (c == '<')
		text += "&lt;";
	else if (c == '>')
		text += "&gt;";
	else
		return false;
	return true;
}

inline void
TextFormatter::set_html_tag(QString& text, bool& isOpened, const QString& openTag, const QString& closeTag)
{
	if (isOpened)
	{
		text += closeTag;
		isOpened = false;
	}
	else
	{
		text += openTag;
		isOpened = true;
	}
}

inline void
TextFormatter::get_irc_color(int& color, bool& isColor, const QString& text, int& i)
{
	int tmpColor = -1;
	// Next char is a number
	if ((i + 1) < text.length() && (tmpColor = text.at(i + 1).digitValue()) != -1)
	{
		color = tmpColor;
		isColor = true;
		// Next char is also a number
		if ((++i + 1) < text.length() && (tmpColor = text.at(i + 1).digitValue()) != -1)
		{
			++i;
			color = color * 10 + tmpColor;
		}
	}
}

inline void
TextFormatter::set_irc_color(int fg, int bg, bool opened, QString& text)
{
	if (opened)
		text += "</span>";
	QString htmlColor = Parameters::get()->ircColor(fg);
	QString htmlBgColor = Parameters::get()->ircColor(bg);
	text += "<span style=\"color:" + htmlColor + "; background-color:" + htmlBgColor + ";\">";
	opened = true;
}

#endif /* !TEXT_FORMATTER_HH */
