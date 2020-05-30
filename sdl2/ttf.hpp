#pragma once

#include <string>
#include <stdexcept>

#include <SDL_ttf.h>


namespace sdl
{

    class Font
    {
    public:
        Font(const char* str_font_name, int const font_pixel_size)
            : font_{ TTF_OpenFont(str_font_name, font_pixel_size) }
        {
            if (this->font_ == nullptr) {
                throw std::exception{ "Fail to Load Font" };
            }
        }

        Font(std::string const str_font_name, int const font_pixel_size)
            : Font{ str_font_name.c_str(), font_pixel_size }
        { }

        ~Font() {
            if (font_ != nullptr) {
                TTF_CloseFont(this->font_);
                this->font_ = nullptr;
            }
        }

        TTF_Font* ptr() const noexcept {
            return this->font_;
        }

        Font(Font&& other) noexcept {
            *this = std::move(other);
        }

        Font& operator=(Font&& other) noexcept {
            if (font_ != other.font_) {

                if (this->font_ != nullptr) {
                    TTF_CloseFont(this->font_);
                }

                this->font_ = other.font_;
                other.font_ = nullptr;
            }
            return *this;
        }

        Font() = default;

        Font(Font const&) = delete;
        Font& operator=(Font const&) = delete;


        SDL_Texture* make_text_texture(const char* str_text, SDL_Renderer* p_render, SDL_Color fg = { 255,255,255,255 }) const noexcept {
            // ASCII 를 받아서 텍스쳐 생성 후 반환

            SDL_Surface* tmp_surface = TTF_RenderText_Solid(this->font_, str_text, fg);
            if (!tmp_surface) {
                return nullptr;
            }

            SDL_Texture* font_texture = SDL_CreateTextureFromSurface(p_render, tmp_surface);
            SDL_FreeSurface(tmp_surface);

            return font_texture;
        }

        SDL_Texture* make(std::u8string const& u8str_text, SDL_Renderer* p_render, SDL_Color fg = { 255,255,255,255 }) const noexcept {

            SDL_Surface* tmp_surface = TTF_RenderUTF8_Solid(this->font_, this->convert_to_string(u8str_text).c_str(), fg);
            if (!tmp_surface) {
                return nullptr;
            }

            SDL_Texture* font_texture = SDL_CreateTextureFromSurface(p_render, tmp_surface);
            SDL_FreeSurface(tmp_surface);

            return font_texture;
        }

        SDL_Texture* make(const char8_t* u8char_text, SDL_Renderer* p_render, SDL_Color fg = { 255,255,255,255 }) const noexcept {

            SDL_Surface* tmp_surface = TTF_RenderUTF8_Solid(this->font_, this->convert_to_string(u8char_text).c_str(), fg);
            if (!tmp_surface) {
                return nullptr;
            }

            SDL_Texture* font_texture = SDL_CreateTextureFromSurface(p_render, tmp_surface);
            SDL_FreeSurface(tmp_surface);

            return font_texture;
        }

        void size_text(std::string const& str, int& w, int& h) const noexcept {
            TTF_SizeText(this->font_, str.c_str(), &w, &h);
        }
        void size_text(std::u8string const& u8str, int& w, int& h) const noexcept {
            this->size_text(this->convert_to_string(u8str).c_str(), w, h);
        }

        std::u8string convert_to_u8string(std::string const string) const noexcept {
            return { string.begin(), string.end() };
        }

    private:
        TTF_Font* font_ = nullptr;

    private:

        std::string convert_to_string(std::u8string const u8string) const noexcept {
            return { u8string.begin(), u8string.end() };
        }

        std::string convert_to_string(const char8_t* u8string) const noexcept {
            return this->convert_to_string(u8string);
        }
    };

} // namespace sdl
