#include "ekg/ui/abstract/ui_abstract_widget.hpp"
#include "ekg/util/env.hpp"

ekg::ui::abstract_widget::abstract_widget() {

}

ekg::ui::abstract_widget::~abstract_widget() {

}

void ekg::ui::abstract_widget::destroy() {

}

void ekg::ui::abstract_widget::on_reload() {

}

void ekg::ui::abstract_widget::on_pre_event(SDL_Event &sdl_event) {
    if (ekg::was_pressed() || ekg::was_released() || ekg::was_motion()) {
        this->flag.hovered = ekg::rect_collide_vec(this->rect, ekg::interact()) && (this->data->get_parent_id() == 0 || ekg::rect_collide_vec(this->parent, ekg::interact()));
    }
}

void ekg::ui::abstract_widget::on_event(SDL_Event &sdl_event) {

}

void ekg::ui::abstract_widget::on_post_event(SDL_Event &sdl_event) {

}

void ekg::ui::abstract_widget::on_update() {

}

void ekg::ui::abstract_widget::on_draw_refresh() {

}
