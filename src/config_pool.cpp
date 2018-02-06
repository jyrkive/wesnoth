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

#include "config_pool.hpp"

#include <iterator>

const int DEFAULT_POOL_SIZE = 16384;

config_pool::config_pool()
	: pool_(DEFAULT_POOL_SIZE)
	, free_configs_()
{
	for(config& c : pool_) {
		free_configs_.push(&c);
	}
}

std::unique_ptr<config, std::function<void(config*)>> config_pool::allocate()
{
	std::stack<config*>& free_configs = instance().free_configs_;
	if(free_configs.empty()) {
		std::list<config>& pool = instance().pool_;
		size_t old_size = pool.size();
		pool.resize(2 * old_size);
		for(auto it = std::next(pool.begin(), old_size); it != pool.end(); ++it) {
			free_configs.push(&*it);
		}
	}

	config& c = *free_configs.top();
	free_configs.pop();

	return std::unique_ptr<config, std::function<void(config*)>>(&c, &release);
}

void config_pool::release(config* c) {
	instance().free_configs_.push(c);
	c->clear();
}

config_pool& config_pool::instance() {
	static config_pool* pool = new config_pool();
	return *pool;
}
