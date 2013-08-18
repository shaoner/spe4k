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
 * \file window.hh
 * \author shaoner
 * \brief Window interface
 */
#ifndef WINDOW_HH
# define WINDOW_HH

# include <QWidget>
# include <QMdiArea>
# include <QMdiSubWindow>
# include <QTextBrowser>

# include "window-item.hh"
# include "text-box.hh"

namespace irc
{
	class Session;
	class CommandEvent;
	class RawEvent;
}

class TextDisplay;
class Parameters;

class Window : public QWidget
{

    Q_OBJECT

public:
	/// Ctor
    Window(const QString& name,
		   irc::Session& session,
		   QWidget* parent = 0);
	/// Dtor
    ~Window();
public:
    void inputFocus();
	/// Display information
	void display(const QString& text);
    void display_event(TextDisplay& textDisplay);
    void display_message(const QString& nick, const QString& tag, const QString& text);
    void display_action(const QString& nick, const QString& tag, const QString& text);
    void display_info(const QString& text);
	/// Parse IRC internal command
    void parse_command(QString& text);
	/// Clear the screen
	void clear();
	bool is_active() const;
    const QString& name() const;
	WindowItem* item() const;
public:
	irc::Session& session;
public slots:
	void cmd();
    void send(const QString& fullnick, const QString& tag);
protected:
	TextBox* _textBox;
    QString _name;
	Parameters* _params;
    WindowItem* _item;
private:
    QList<QString> _history;
    int _historyIdx;
};

inline bool
Window::is_active() const
{
	return (_item->subwindow()->mdiArea()->activeSubWindow() == _item->subwindow());
}

inline const QString&
Window::name() const
{
	return _name;
}

inline WindowItem*
Window::item() const
{
	return _item;
}

inline void
Window::clear()
{
	_textBox->screen->clear();
}

#endif /* !WINDOW_HH */
