/* $Id$ */
/*
   Copyright (C) 2003-5 by David White <davidnwhite@verizon.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef WIDGET_HPP_INCLUDED
#define WIDGET_HPP_INCLUDED

#include "../events.hpp"
#include "../sdl_utils.hpp"

#include "SDL.h"

#include <string>
#include <vector>

class CVideo;

namespace gui {

enum ALIGN { LEFT_ALIGN, CENTER_ALIGN, RIGHT_ALIGN };

class widget : public events::handler
{
public:
	SDL_Rect const &location() const;
	virtual void set_location(SDL_Rect const &rect);
	void set_location(int x, int y);
	void set_width(unsigned w);
	void set_height(unsigned h);
	void set_measurements(unsigned w, unsigned h);
	void set_align(ALIGN a);

	unsigned width() const;
	unsigned height() const;
	ALIGN align() const;

	virtual bool focus() const;
	void set_focus(bool focus);

	virtual void hide(bool value = true);
	bool hidden() const;
	virtual void enable(bool new_val = true);
	bool enabled() const;

	void set_clip_rect(const SDL_Rect& rect);

	//Function to set the widget to draw in 'volatile' mode.
	//When in 'volatile' mode, instead of using the normal
	//save-background-redraw-when-dirty procedure, redrawing is done
	//every frame, and then after every frame the area under the widget
	//is restored to the state it was in before the frame. This is useful
	//for drawing widgets with alpha components in volatile settings where
	//the background may change at any time.
	//(e.g. for putting widgets on top of the game map)
	void set_volatile(bool val=true);

	void set_dirty(bool dirty=true);
	bool dirty() const;

	void set_help_string(const std::string& str);

	virtual void process_help_string(int mousex, int mousey);

protected:
	widget(widget const &o);
	widget(CVideo& video, const bool auto_join=true);
	virtual ~widget();

	// During each relocation, this function should be called to register
	// the rectangles the widget needs to refresh automatically
	void bg_register(SDL_Rect const &rect);
	void bg_restore() const;
	void bg_restore(SDL_Rect const &rect) const;
	void bg_update();
	void bg_cancel();

	CVideo& video() const { return *video_; };

	virtual void handle_event(SDL_Event const &/*event*/) {}

	virtual void draw();
	virtual void draw_contents() {};
	virtual void update_location(SDL_Rect const &rect);

	const SDL_Rect* clip_rect() const;

private:
	void volatile_draw();
	void volatile_undraw();

	CVideo* video_;
	std::vector< surface_restorer > restorer_;
	SDL_Rect rect_;
	bool focus_;		// Should user input be ignored?
	mutable bool needs_restore_; // Have we drawn ourselves, so that if moved, we need to restore the background?

	enum { UNINIT, HIDDEN, DIRTY, DRAWN } state_;
	bool enabled_;
	bool clip_;
	SDL_Rect clip_rect_;

	bool volatile_;

	std::string help_text_;
	int help_string_;
	ALIGN align_; //limited support, use position

	friend class scrollpane;
};

}

#endif
