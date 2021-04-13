#include "Table.h"

#include <codecvt>
#include <locale>

using namespace game::common;
using namespace game::visual;

TableColumn::TableColumn(int width, std::string title, render_lambda_t render_lambda)
    : width(width)
    , title(std::move(title))
    , render_lambda(std::move(render_lambda)) {
}

int TableColumn::GetWidth() const {
  return width;
}

game::visual::ColoredCharMatrix TableColumn::RenderCell(const TableRow *row) const {
  game::visual::ColoredCharMatrix rendered_field(common::coordinate_size_t(width, 1));

  render_lambda(row, rendered_field);

  return rendered_field;
}

game::visual::ColoredCharMatrix TableColumn::RenderTitle() const {
  game::visual::ColoredCharMatrix rendered_title(common::coordinate_size_t(width, 1));

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  rendered_title.SetString(converter.from_bytes(title), Position(0, 0));

  return rendered_title;
}
