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
 * \file text-formatter.cc
 * \author shaoner
 * \brief IRC text utils
 */
#include "text-formatter.hh"

QString
TextFormatter::html_escape(const QString& text)
{
	QString html;
	html.reserve(int(text.length() * 1.1));
	QChar c;

	for (int i = 0, len = text.length(); i < len; ++i)
	{
		c = text.at(i);
		if (!escape(c, html))
			html += c;
	}
	return html;
}

QString
TextFormatter::irc_to_html(const QString& raw)
{
	bool openBold = false;
	bool openColor = false;
	bool openItalic = false;
	bool openUnderline = false;
	bool openStrikethrough = false;
	bool openReverse = false;
	int fgcol = 1;
	int bgcol = 0;
	QString text;
	QChar c;
	text.reserve(int(raw.length() * 1.1));
	for (int i = 0, len = raw.size(); i < len; ++i)
	{
		c = raw.at(i);
		if (c == '&')
			text += "&amp;";
		else if (c == '<')
			text += "&lt;";
		else if (c == '>')
			text += "&gt;";
		else if (c == '\x02')
			set_html_tag(text, openBold, "<b>", "</b>");
		else if (c == CODE_IRC_ITALIC)
			set_html_tag(text, openItalic, "<i>", "</i>");
		else if (c == CODE_IRC_UNDERLINE)
			set_html_tag(text, openUnderline, "<u>", "</u>");
		else if (c == CODE_IRC_STRIKETHROUGH)
			set_html_tag(text, openUnderline, "<s>", "</s>");
		else if (c == CODE_IRC_COLOR)
		{
			bool isStartColor = false;
			get_irc_color(fgcol, isStartColor, raw, i);
			// Background color
			if ((i + 1) < len && raw[i + 1] == ',')
			{
				get_irc_color(bgcol, isStartColor, raw, ++i);
			}
			if (isStartColor)
			{
				set_irc_color(fgcol, bgcol, openColor, text);
			}
			else if (openColor)
			{
				text += "</span>";
				openColor = false;
			}
		}
		else if (c == CODE_IRC_REVERSE)
		{
			if (openReverse)
				set_irc_color(fgcol, bgcol, openColor, text);
			else
				set_irc_color(bgcol, fgcol, openColor, text);
			openReverse = !openReverse;
		}
		else if (c == CODE_IRC_RESET)
		{
			if (openColor)
			{
				text += "</span>";
				openColor = false;
			}
			if (openBold)
			{
				text += "</b>";
				openBold = false;
			}
			if (openItalic)
			{
				text += "</i>";
				openItalic = false;
			}
			if (openUnderline)
			{
				text += "</u>";
				openUnderline = false;
			}
			if (openStrikethrough)
			{
				text += "</s>";
				openStrikethrough = false;
			}
		}
		else
			text += c;
	}
	if (openColor)
		text += "</span>";
	if (openBold)
		text += "</b>";
	if (openItalic)
		text += "</i>";
	if (openUnderline)
		text += "</u>";
	if (openStrikethrough)
		text += "</s>";
	return text;
}
