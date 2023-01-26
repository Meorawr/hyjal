// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

namespace meorawr::hyjal {
    template<typename Tag>
    struct intrusive_list_node_traits;

    template<typename Tag>
    class intrusive_list_node {
    public:
        constexpr intrusive_list_node() noexcept = default;
        intrusive_list_node(const intrusive_list_node&) = delete;
        intrusive_list_node(intrusive_list_node&&) = delete;

        intrusive_list_node& operator=(const intrusive_list_node&) = delete;
        intrusive_list_node& operator=(intrusive_list_node&&) = delete;

        constexpr ~intrusive_list_node() noexcept = default;

    private:
        intrusive_list_node* prev_ = nullptr;
        intrusive_list_node* next_ = nullptr;

        friend intrusive_list_node_traits<Tag>;
    };

    template<typename Tag>
    struct intrusive_list_node_traits {
        using node = intrusive_list_node<Tag>;
        using node_ptr = node*;
        using const_node_ptr = const node*;

        static constexpr node_ptr get_previous(const_node_ptr n) noexcept { return n->prev_; }
        static constexpr node_ptr get_next(const_node_ptr n) noexcept { return n->next_; }

        static constexpr void set_previous(node_ptr n, node_ptr prev) noexcept { n->prev_ = prev; }
        static constexpr void set_next(node_ptr n, node_ptr next) noexcept { n->next_ = next; }
    };
}
