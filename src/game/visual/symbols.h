#ifndef CROSSING_CONSOLES_SYMBOLS_H
#define CROSSING_CONSOLES_SYMBOLS_H

namespace game::visual::symbols {
const wchar_t box_drawings_double_horizontal = L'\u2550';
const wchar_t box_drawings_double_vertical = L'\u2551';
const wchar_t box_drawings_double_down_and_right = L'\u2554';
const wchar_t box_drawings_double_down_and_left = L'\u2557';
const wchar_t box_drawings_double_up_and_right = L'\u255A';
const wchar_t box_drawings_double_up_and_left = L'\u255D';
const wchar_t box_drawings_double_vertical_and_right = L'\u2560';
const wchar_t box_drawings_double_vertical_and_left = L'\u2563';

const wchar_t box_drawings_light_horizontal = L'\u2500';
const wchar_t box_drawings_light_down_and_right = L'\u250C';
const wchar_t box_drawings_light_down_and_left = L'\u2510';
const wchar_t box_drawings_light_up_and_right = L'\u2514';
const wchar_t box_drawings_light_up_and_left = L'\u2518';
const wchar_t box_drawings_light_vertical_and_horizontal = L'\u253C';
const wchar_t box_drawings_light_left = L'\u2574';
const wchar_t box_drawings_light_right = L'\u2576';

const wchar_t box_drawings_heavy_horizontal = L'\u2501';
const wchar_t box_drawings_heavy_vertical = L'\u2503';
const wchar_t box_drawings_heavy_down_and_right = L'\u250F';
const wchar_t box_drawings_heavy_down_and_left = L'\u2513';
const wchar_t box_drawings_heavy_up_and_right = L'\u2517';
const wchar_t box_drawings_heavy_up_and_left = L'\u251B';
const wchar_t box_drawings_heavy_horizontal_and_down = L'\u2533';
const wchar_t box_drawings_heavy_horizontal_and_up = L'\u253B';
const wchar_t box_drawings_heavy_vertical_and_right = L'\u2523';
const wchar_t box_drawings_heavy_vertical_and_left = L'\u252B';
const wchar_t box_drawings_heavy_vertical_and_horizontal = L'\u254B';

const wchar_t full_block = L'\u2588';
const wchar_t light_shade = L'\u2591';
const wchar_t medium_shade = L'\u2592';
const wchar_t dark_shade = L'\u2593';
const wchar_t white_circle = L'\u25CB';

const wchar_t black_heart_suit = L'\u2665';  // rendered as an extra wide character
const wchar_t coptic_capital_letter_dei = L'\u03EE';
const wchar_t not_sign = L'\u00AC';
const wchar_t bullet = L'\u2022';

const wchar_t black_vertical_ellipse = L'\u2B2E';

const std::wstring title_top /*  */ = L" ▄▀▀ █▀▄ ▄▀▄ ▄▀▀ ▄▀▀ █ █▄ █ ▄▀▀   ▄▀▀ ▄▀▄ █▄ █ ▄▀▀ ▄▀▄ █   ██▀ ▄▀▀";
const std::wstring title_down /* */ = L" ▀▄▄ █▀▄ ▀▄▀ ▄██ ▄██ █ █ ▀█ ▀▄█   ▀▄▄ ▀▄▀ █ ▀█ ▄██ ▀▄▀ █▄▄ █▄▄ ▄██";

}  // namespace game::visual::symbols

#endif  // CROSSING_CONSOLES_SYMBOLS_H
