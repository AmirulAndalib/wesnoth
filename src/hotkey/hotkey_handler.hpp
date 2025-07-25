/*
	Copyright (C) 2014 - 2025
	by Chris Beck <render787@gmail.com>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

/**
 * @file
 * This file implements all the hotkey handling and menu details for
 * play controller.
 */

#pragma once

#include "play_controller.hpp"

namespace events { class menu_handler; }
namespace events { class mouse_handler; }
namespace game_events { class wml_menu_item; }

class game_state;
class saved_game;

class team;

class play_controller::hotkey_handler : public hotkey::command_executor_default {

protected:
	display& get_display() override { return play_controller_.get_display(); }

	/** References to parent object / constituents */
	play_controller & play_controller_;

	events::menu_handler & menu_handler_;
	events::mouse_handler & mouse_handler_;
	game_display * gui() const;
	saved_game & saved_game_;
	game_state & gamestate();
	const game_state & gamestate() const;

private:
	//
	// Private data related to menu implementation (expansion of AUTOSAVES, WML entries)
	//

	// Expand AUTOSAVES in the menu items, setting the real savenames.
	void expand_autosaves(std::vector<config>& items) const;
	void expand_quickreplay(std::vector<config>& items) const;

	/**
	 * Replaces "wml" in @a items with all active WML menu items for the current field.
	 */
	void expand_wml_commands(std::vector<config>& items);

protected:
	bool browse() const;
	bool linger() const;

public:
	hotkey_handler(play_controller &, saved_game &);
	~hotkey_handler();

	static const std::string wml_menu_hotkey_prefix;

	//event handlers, overridden from command_executor
	virtual void objectives() override;
	virtual void show_statistics() override;
	virtual void unit_list() override;
	virtual void move_action() override;
	virtual void select_and_action() override;
	virtual void touch_hex() override;
	virtual void select_hex() override;
	virtual void deselect_hex() override;
	virtual void status_table() override;
	virtual void save_game() override;
	virtual void save_replay() override;
	virtual void save_map() override;
	virtual void load_game() override;
	virtual void preferences() override;
	virtual void speak() override;
	virtual void show_chat_log() override;
	virtual void show_help() override;
	virtual void cycle_units() override;
	virtual void cycle_back_units() override;
	virtual void undo() override;
	virtual void redo() override;
	virtual void show_enemy_moves(bool ignore_units) override;
	virtual void goto_leader() override;
	virtual void unit_description() override;
	virtual void terrain_description() override;
	virtual void toggle_ellipses() override;
	virtual void toggle_grid() override;
	virtual void search() override;
	virtual void toggle_accelerated_speed() override;
	virtual void scroll_up(bool on) override;
	virtual void scroll_down(bool on) override;
	virtual void scroll_left(bool on) override;
	virtual void scroll_right(bool on) override;
	virtual void replay_skip_animation() override
	{ return play_controller_.toggle_skipping_replay(); }

	virtual void load_autosave(const std::string& filename, bool start_replay = false);
	virtual hotkey::action_state get_action_state(const hotkey::ui_command&) const override;
	/** Check if a command can be executed. */
	virtual bool can_execute_command(const hotkey::ui_command& command) const override;
	virtual bool do_execute_command(const hotkey::ui_command& command, bool press=true, bool release=false) override;
	void show_menu(const std::vector<config>& items_arg, const point& menu_loc, bool context_menu) override;

	/** Inherited from command_executor. */
	bool in_context_menu(const hotkey::ui_command& cmd) const override;
};
