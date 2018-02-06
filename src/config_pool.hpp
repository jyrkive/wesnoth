/*
   Copyright (C) 2018 by Jyrki Vesterinen <sandgtx@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#include "config.hpp"
#include <functional>
#include <list>
#include <memory>
#include <stack>

class config_pool
{
public:
	config_pool();

	static std::unique_ptr<config, std::function<void(config*)>> allocate();
	static void release(config* c);

private:
	std::list<config> pool_;
	std::stack<config*> free_configs_;

	static config_pool& instance();
};
