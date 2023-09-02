/*
 * VOKEGPU EKG LICENSE
 *
 * Respect ekg license policy terms, please take a time and read it.
 * 1- Any "skidd" or "stole" is not allowed.
 * 2- Forks and pull requests should follow the license policy terms.
 * 3- For commercial use, do not sell without give credit to vokegpu ekg.
 * 4- For ekg users and users-programmer, we do not care, free to use in anything (utility, hacking, cheat, game, software).
 * 5- Malware, rat and others virus. We do not care.
 * 6- Do not modify this license under any instance.
 *
 * @VokeGpu 2023 all rights reserved.
 */

#ifndef EKG_UI_ABSTRACT_H
#define EKG_UI_ABSTRACT_H

#include "ekg/util/geometry.hpp"
#include "ekg/util/aspect.hpp"
#include <vector>

namespace ekg {
    enum class type {
        abstract, frame, button, label, slider, slider2d, checkbox, textbox, entrybox, combobox, tab, popup, scroll
    };

    enum class uisync {
        reset = 1, dimension = 2
    };

    enum attribute {
        separator = 1 << 2
    };

    namespace ui {
        class component {
        public:
            std::string name {};
            ekg::rect rect_dimension {};
            ekg::rect rect_content {};
            uint64_t id {};
            uint64_t linked_id {};
            uint16_t attributes {};
        };

        class abstract {
        protected:
            int32_t id {};
            int32_t parent_id {};
            std::vector<int32_t> child_id_list {};

            bool alive {true};
            uint16_t dock_flags {};
            uint16_t sync_flags {};
            std::string tag {};

            ekg::state state {};
            ekg::type type {ekg::type::abstract};
            ekg::rect rect_widget {};
            ekg::rect sync_ui {};
        
            int32_t scaled_height {};
        public:
            abstract();
            ~abstract();

            ekg::ui::abstract *set_tag(std::string_view);
            std::string get_tag();

            ekg::ui::abstract *add_child(int32_t);
            std::vector<int32_t> &get_child_id_list();
            ekg::ui::abstract *remove_child(int32_t);

            ekg::ui::abstract *set_id(int32_t);
            int32_t get_id();

            ekg::ui::abstract *set_parent_id(int32_t);
            int32_t get_parent_id();

            ekg::ui::abstract *set_alive(bool);
            bool is_alive();
            void destroy();

            ekg::ui::abstract *set_state(const ekg::state&);
            ekg::state get_state();

            ekg::ui::abstract *set_type(const ekg::type&);
            ekg::type get_type();

            uint16_t get_place_dock();
            uint16_t &get_sync();
            void reset();

            ekg::rect &widget();
            ekg::rect &ui();

            ekg::ui::abstract *set_scaled_height_layout(int32_t);
            bool has_parent();
            bool has_children();
        };
    }
}

#endif