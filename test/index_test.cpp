// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "../src/lua/index.hpp"

using meorawr::hyjal::lua::absolute_index;
using meorawr::hyjal::lua::accessible_index;
using meorawr::hyjal::lua::index_difference_t;
using meorawr::hyjal::lua::index_t;
using meorawr::hyjal::lua::pseudo_index;
using meorawr::hyjal::lua::raw_index;
using meorawr::hyjal::lua::relative_index;
using meorawr::hyjal::lua::stack_index;
using meorawr::hyjal::lua::upvalue_index;

// clang-format off

template<typename I>
concept is_basic_index = std::convertible_to<I, index_t> && requires(I index, index_t i) {
    typename I::category_type;
    typename I::concept_type;
    
    { index == index } noexcept -> std::same_as<bool>;
    { index != index } noexcept -> std::same_as<bool>;
    
    { index == i } noexcept -> std::same_as<bool>;
    { index != i } noexcept -> std::same_as<bool>;
    { i == index } noexcept -> std::same_as<bool>;
    { i != index } noexcept -> std::same_as<bool>;
};

template<typename I>
concept is_linear_index = is_basic_index<I> && requires(I index, index_t i, index_difference_t n) {
    { index < index } noexcept -> std::same_as<bool>;
    { index > index } noexcept -> std::same_as<bool>;
    { index <= index } noexcept -> std::same_as<bool>;
    { index >= index } noexcept -> std::same_as<bool>;
    { index <=> index } noexcept -> std::same_as<std::strong_ordering>;

    { index < i } noexcept -> std::same_as<bool>;
    { index > i } noexcept -> std::same_as<bool>;
    { index <= i } noexcept -> std::same_as<bool>;
    { index >= i } noexcept -> std::same_as<bool>;
    { index <=> i } noexcept -> std::same_as<std::strong_ordering>;

    { i < index } noexcept -> std::same_as<bool>;
    { i > index } noexcept -> std::same_as<bool>;
    { i <= index } noexcept -> std::same_as<bool>;
    { i >= index } noexcept -> std::same_as<bool>;
    { i <=> index } noexcept -> std::same_as<std::strong_ordering>;
    
    { ++index } noexcept -> std::same_as<I&>;
    { index++ } noexcept -> std::same_as<I>;
    { --index } noexcept -> std::same_as<I&>;
    { index-- } noexcept -> std::same_as<I>;
    { index += n } noexcept -> std::same_as<I&>;
    { index -= n } noexcept -> std::same_as<I&>;
    { index + n } noexcept -> std::same_as<I>;
    { index - n } noexcept -> std::same_as<I>;
    { index - index } noexcept -> std::same_as<index_difference_t>;
};

template<typename I>
struct categorized_index {
    struct category_type : I::category_type {};
    using concept_type = typename I::concept_type;

    index_t i = {};
};

template<typename I>
concept is_category_constructible = std::constructible_from<I, categorized_index<I>>;

// clang-format on

// An index is "basic" if it supports equality comparison operators, has the
// required typedefs, and is convertible to an integral index ('index_t').

static_assert(is_basic_index<raw_index>);
static_assert(is_basic_index<accessible_index>);
static_assert(is_basic_index<stack_index>);
static_assert(is_basic_index<absolute_index>);
static_assert(is_basic_index<relative_index>);
static_assert(is_basic_index<pseudo_index>);
static_assert(is_basic_index<upvalue_index>);

// An index is "linear" if it supports ordered comparison operators as well
// as basic arithmetic operations. These operations are provided by default
// if the 'concept_type' is set a derivative of 'linear_index_tag'.

static_assert(!is_linear_index<raw_index>);
static_assert(!is_linear_index<accessible_index>);
static_assert(is_linear_index<stack_index>);
static_assert(is_linear_index<absolute_index>);
static_assert(is_linear_index<relative_index>);
static_assert(!is_linear_index<pseudo_index>);
static_assert(is_linear_index<upvalue_index>);

// An index type is "category-constructible" if it can be constructed from
// an instance of another index type whose 'category_type' is a derivative
// of to to-be-constructed type.

static_assert(!is_category_constructible<raw_index>);
static_assert(is_category_constructible<accessible_index>);
static_assert(is_category_constructible<stack_index>);
static_assert(!is_category_constructible<absolute_index>);
static_assert(!is_category_constructible<relative_index>);
static_assert(!is_category_constructible<pseudo_index>);
static_assert(!is_category_constructible<upvalue_index>);
