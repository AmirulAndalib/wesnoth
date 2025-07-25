/*
	Copyright (C) 2010 - 2025
	by Mark de Wever <koraq@xs4all.nl>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

#include "gui/widgets/scrollbar_container.hpp"
#include "gui/widgets/tree_view_node.hpp"

namespace gui2
{

namespace implementation {
	struct builder_tree_view;
	struct tree_node
	{
		explicit tree_node(const config& cfg);

		std::string id;
		bool unfolded;
		builder_grid_ptr builder;
	};
}

// ------------ WIDGET -----------{

class tree_view : public scrollbar_container
{
	friend struct implementation::builder_tree_view;
	friend struct implementation::tree_node;
	friend class tree_view_node;

public:
	typedef implementation::tree_node node_definition;

	explicit tree_view(const implementation::builder_tree_view& builder);

	~tree_view();

	using scrollbar_container::finalize_setup;

	const tree_view_node& get_root_node() const
	{
		return *root_node_;
	}

	tree_view_node& get_root_node()
	{
		return *root_node_;
	}

	tree_view_node& add_node(
		const std::string& id, const widget_data& data, const int index = -1);

	/**
	 * Removes the given node as a child of its parent node.
	 *
	 * @param node      A pointer to the node to remove.
	 *
	 * @returns         A pair consisting of a smart pointer managing the removed
	 *                  node, and its position before removal.
	 */
	std::pair<std::shared_ptr<tree_view_node>, int> remove_node(tree_view_node* node);

	void clear();

	/** See @ref container_base::set_self_active. */
	virtual void set_self_active(const bool active) override;

	bool empty() const;

	/** See @ref widget::layout_children. */
	virtual void layout_children() override;

	/***** ***** ***** setters / getters for members ***** ****** *****/

	void set_indentation_step_size(const unsigned indentation_step_size)
	{
		indentation_step_size_ = indentation_step_size;
	}

	unsigned get_indentation_step_size() const
	{
		return indentation_step_size_;
	}

	tree_view_node* selected_item()
	{
		return selected_item_;
	}

	const tree_view_node* selected_item() const
	{
		return selected_item_;
	}

	const std::vector<node_definition>& get_node_definitions() const
	{
		return node_definitions_;
	}

protected:
	/***** ***** ***** ***** keyboard functions ***** ***** ***** *****/

	/** Inherited from scrollbar_container. */
	void handle_key_up_arrow(SDL_Keymod modifier, bool& handled) override;

	/** Inherited from scrollbar_container. */
	void handle_key_down_arrow(SDL_Keymod modifier, bool& handled) override;

	/** Inherited from scrollbar_container. */
	void handle_key_left_arrow(SDL_Keymod modifier, bool& handled) override;

	/** Inherited from scrollbar_container. */
	void handle_key_right_arrow(SDL_Keymod modifier, bool& handled) override;

private:
	static inline const std::string root_node_id = "root";

	/**
	 * @todo evaluate which way the dependency should go.
	 *
	 * We no depend on the implementation, maybe the implementation should
	 * depend on us instead.
	 */
	const std::vector<node_definition> node_definitions_;

	unsigned indentation_step_size_;

	bool need_layout_;

	tree_view_node* root_node_;

	tree_view_node* selected_item_;

	/**
	 * Resizes the content.
	 *
	 * The resize either happens due to resizing the content or invalidate the
	 * layout of the window.
	 *
	 * @param width_modification  The wanted modification to the width:
	 *                            * negative values reduce width.
	 *                            * zero leave width as is.
	 *                            * positive values increase width.
	 * @param height_modification The wanted modification to the height:
	 *                            * negative values reduce height.
	 *                            * zero leave height as is.
	 *                            * positive values increase height.
	 * @param width_modification_pos
	 * @param height_modification_pos
	 */
	void resize_content(const int width_modification,
						const int height_modification,
						const int width_modification_pos = -1,
						const int height_modification_pos = -1);

	/** Layouts the children if needed. */
	void layout_children(const bool force);

	/** Inherited from container_base. */
	virtual void finalize_setup();

public:
	/** Static type getter that does not rely on the widget being constructed. */
	static const std::string& type();

	/** Optionally returns the node definition with the given id, or nullopt if not found. */
	utils::optional<decltype(node_definitions_)::const_iterator> get_node_definition(const std::string& id) const
	{
		const auto def = utils::ranges::find(node_definitions_, id, &node_definition::id);
		return def != node_definitions_.end() ? utils::make_optional(def) : utils::nullopt;
	}

private:
	/** Inherited from styled_widget, implemented by REGISTER_WIDGET. */
	virtual const std::string& get_control_type() const override;

	/***** ***** ***** signal handlers ***** ****** *****/

	void signal_handler_left_button_down(const event::ui_event event);

	template<tree_view_node* (tree_view_node::*func) ()>
	tree_view_node* get_next_node();

	template<tree_view_node* (tree_view_node::*func) ()>
	bool handle_up_down_arrow();
};

// }---------- DEFINITION ---------{

struct tree_view_definition : public styled_widget_definition
{

	explicit tree_view_definition(const config& cfg);

	struct resolution : public resolution_definition
	{
		explicit resolution(const config& cfg);

		builder_grid_ptr grid;
	};
};

// }---------- BUILDER -----------{

namespace implementation
{

struct builder_tree_view : public builder_scrollbar_container
{
	explicit builder_tree_view(const config& cfg);

	using builder_styled_widget::build;

	virtual std::unique_ptr<widget> build() const override;

	unsigned indentation_step_size;

	/**
	 * The types of nodes in the tree view.
	 *
	 * Since we expect the amount of nodes to remain low it's stored in a
	 * vector and not in a map.
	 */
	std::vector<tree_node> nodes;

	/*
	 * NOTE this class doesn't have a data section, so it can only be filled
	 * with data by the engine. I think this poses no limit on the usage since
	 * I don't foresee that somebody wants to pre-fill a tree view. If the need
	 * arises the data part can be added.
	 */
};

} // namespace implementation

// }------------ END --------------

} // namespace gui2
