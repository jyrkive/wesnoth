/*
   Copyright (C) 2003 - 2016 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef REPORTS_HPP_INCLUDED
#define REPORTS_HPP_INCLUDED

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include "display_context.hpp"

#include <vector>

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/future.hpp>

//this module is responsible for outputting textual reports of
//various game and unit statistics

class gamemap;
class team;
class tod_manager;
class unit_map;

class display;

namespace wb {
	class manager;
}

namespace events {
	class mouse_handler;
}

class reports
{
public:

	class context
	{
	public:
		context(const display_context & dc, display & disp, const tod_manager & tod, boost::shared_ptr<wb::manager> wb, boost::optional<events::mouse_handler &> mhb) : dc_(dc), disp_(disp), tod_(tod), wb_(wb), mhb_(mhb) {}

		const std::vector<team> & teams() const { return dc_.teams(); }
		const unit_map & units() const { return dc_.units(); }
		const gamemap & map() const { return dc_.map(); }

		const display_context & dc() const { return dc_; }
		display & screen() const { return disp_; }
		const tod_manager & tod() const { return tod_; }
		boost::shared_ptr<wb::manager> wb() const { return wb_; }
		boost::optional<events::mouse_handler&> mhb() const { return mhb_; }

	private:
		const display_context & dc_;
		display & disp_;
		const tod_manager & tod_;
		boost::shared_ptr<wb::manager> wb_;
		boost::optional<events::mouse_handler&> mhb_;
	};

	struct generator
	{
		virtual config generate(const context & ct) = 0;
		virtual ~generator() {}
	};

	void register_generator(const std::string &name, generator *);

	config generate_report(const std::string &name, const context & ct, bool only_static = false);
	boost::future<config> generate_report_async(const std::string &name, const context & ct);
	bool is_asynchronous_generator(const std::string &name) const;

	const std::set<std::string> &report_list();


	typedef config (*generator_function)(const reports::context & );
	typedef boost::future<config> (*generator_function_async)(const reports::context &);
	typedef std::map<std::string, boost::shared_ptr<reports::generator> > dynamic_report_generators;

private:

	std::set<std::string> all_reports_;

	dynamic_report_generators dynamic_generators_;

};

#endif
