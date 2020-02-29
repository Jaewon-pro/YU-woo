#pragma once

#include <string>
#include <stdexcept>

#include <SDL_ttf.h>


namespace sdl
{

class Font
{
public:
    Font(const char* str_font_name, const int font_pixel_size)
        : p_ttf_font{ TTF_OpenFont(str_font_name, font_pixel_size) }
    {
        if (this->p_ttf_font == nullptr) {
            throw std::exception{ "Fail to load Font" };
        }
    }

    Font(std::string const& str_font_name, const int font_pixel_size)
        : Font{ str_font_name.c_str(), font_pixel_size }
    { }

    ~Font() noexcept {
        if (p_ttf_font != nullptr) {
            TTF_CloseFont(this->p_ttf_font);
            this->p_ttf_font = nullptr;
        }
    }

    TTF_Font* ptr() {
        return this->p_ttf_font;
    }

    Font(Font&& other) noexcept {
        *this = std::move(other);
    }

    Font& operator=(Font&& other) noexcept {
        if (p_ttf_font != other.p_ttf_font) {

            if (this->p_ttf_font != nullptr) {
                TTF_CloseFont(this->p_ttf_font);
            }

            this->p_ttf_font = other.p_ttf_font;
            other.p_ttf_font = nullptr;
        }
        return *this;
    }

    Font() = default;

    Font(Font const&) = delete;
    Font& operator=(Font const&) = delete;

    
    SDL_Texture* make_text_texture(const char* str_text, SDL_Renderer* p_render, SDL_Color fg = { 255,255,255,255 }) const noexcept {
        // ASCII CHAR

        // 폰트를 불러오고 인자로 받은 문자열을 텍스쳐로 만듦.

        SDL_Surface* tmp_surface = TTF_RenderText_Solid(this->p_ttf_font, str_text, fg);
        if (!tmp_surface) {
            return nullptr;
        }

        SDL_Texture* font_texture = SDL_CreateTextureFromSurface(p_render, tmp_surface);
        SDL_FreeSurface(tmp_surface);

        return font_texture;
    }

    SDL_Texture* make_text_texture(const wchar_t* wstr_text, SDL_Renderer* p_render, SDL_Color fg = { 255,255,255,255 }) const noexcept {
        // wchar_t를 받아서 Uint16 문자로 변환 후,
        // 텍스쳐 생성

        // Uint16 는 unsigned short이다.

        SDL_Surface* tmp_surface = TTF_RenderUNICODE_Solid(this->p_ttf_font, this->convert_to_Uint16(wstr_text), fg);
        if (!tmp_surface) {
            return nullptr;
        }

        SDL_Texture* font_texture = SDL_CreateTextureFromSurface(p_render, tmp_surface);
        SDL_FreeSurface(tmp_surface);

        return font_texture;
    }



    void size_text(std::string const& str_text, int& w, int& h) const noexcept {
        TTF_SizeText(this->p_ttf_font, str_text.c_str(), &w, &h);
    }
    
    void size_text(std::wstring const& wstr_text, int& w, int& h) const noexcept {
        TTF_SizeUNICODE(this->p_ttf_font, this->convert_to_Uint16(wstr_text.c_str()), &w, &h);
    }


    static std::wstring string_to_wstring(std::string const& str) noexcept {
        // 변환 함수가 여기에 있는게 이상하지만
        // string을 wide world string 으로 변환하는 함수
        std::wstring wstr;

        return wstr.assign(str.begin(), str.end());
    }


private:
    TTF_Font* p_ttf_font = nullptr;

private:
    // wchar_t를 받아서 Uint16 문자로 변환
    const Uint16* convert_to_Uint16(const wchar_t LString[]) const noexcept {
        return reinterpret_cast<const Uint16*>(&(LString[0]));
    }
    // https://gamedev.stackexchange.com/questions/152231/how-to-handle-japanese-on-windows-and-linux-using-sdl-ttf-and-c

    const Uint16* convert_to_Uint16(wchar_t LString[]) const noexcept {
        return reinterpret_cast<const Uint16*>(&(LString[0]));
    }

    
};

} // namespace sdl
