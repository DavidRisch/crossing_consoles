#ifndef CROSSING_CONSOLES_TABLE_H
#define CROSSING_CONSOLES_TABLE_H

#include <functional>

#include "../common/Position.h"
#include "../world/World.h"
#include "ColoredCharMatrix.h"
#include "symbols.h"

namespace game::visual {

template <typename R>
class Table;

/**
 * \brief Abstract row of a `Table`.
 */
class TableRow {
 public:
  template <typename R>
  [[nodiscard]] game::visual::ColoredCharMatrix RenderRow(const Table<R> *p) const {
    assert(p != nullptr);

    ColoredCharMatrix row(common::coordinate_size_t(p->CalculateTotalWidth(), 1));

    row.AppendChar(symbols::box_drawings_heavy_vertical);

    for (const auto &column : p->columns) {
      auto field = column.RenderCell(this);
      assert(field.GetSize() == common::coordinate_size_t(column.GetWidth(), 1));

      row.InsertMatrix(field);

      row.AppendChar(symbols::box_drawings_heavy_vertical);
    }

    return row;
  }

 private:
  /**
   * \brief Hack to make this class polymorphic.
   */
  [[maybe_unused]] virtual void make_polymorphic(){};
};

/**
 * \brief Column of a `Table`.
 */
class TableColumn {
 public:
  typedef std::function<void(const TableRow *row, game::visual::ColoredCharMatrix &field)> render_lambda_t;

  TableColumn(int width, std::string title, render_lambda_t render_lambda);

  /**
   * \brief Render a single cell.
   */
  [[nodiscard]] game::visual::ColoredCharMatrix RenderCell(const TableRow *row) const;

  /**
   * \brief Render a single header cell.
   */
  [[nodiscard]] game::visual::ColoredCharMatrix RenderTitle() const;

  [[nodiscard]] int GetWidth() const;

 private:
  const int width;
  const std::string title;
  const render_lambda_t render_lambda;
};

/**
 * \brief An abstract table which can be rendered.
 */
template <typename R>
class Table {
 public:
  [[nodiscard]] virtual std::list<R> MakeRows() const = 0;

  /**
   * \brief Render the table including its border.
   */
  [[nodiscard]] virtual game::visual::ColoredCharMatrix Render() const {
    std::list<R> rows = MakeRows();

    const int divider_height = 1;
    const int header_height = 1;
    const int row_count = divider_height + header_height + divider_height + (int)rows.size() + divider_height;

    ColoredCharMatrix table(common::coordinate_size_t(CalculateTotalWidth(), row_count));

    table.AppendFullWidthMatrix(MakeDivider(true, false));
    table.AppendFullWidthMatrix(MakeHeader());
    table.AppendFullWidthMatrix(MakeDivider(false, false));

    for (const auto &row : rows) {
      auto rendered_row = row.RenderRow(this);

      table.AppendFullWidthMatrix(rendered_row);
    }

    table.AppendFullWidthMatrix(MakeDivider(false, true));

    return table;
  };

  /**
   * \brief Calculate the width in chars of all columns and dividers/borders.
   */
  [[nodiscard]] int CalculateTotalWidth() const {
    int all_column_widths =
        std::accumulate(columns.begin(), columns.end(), 0,
                        [](int current, const TableColumn &column) { return current + column.GetWidth(); });
    int line_count = static_cast<int>(columns.size()) + 1;
    return all_column_widths + line_count;
  };

 protected:
  [[nodiscard]] game::visual::ColoredCharMatrix MakeHeader() const {
    ColoredCharMatrix row(common::coordinate_size_t(CalculateTotalWidth(), 1));

    row.AppendChar(symbols::box_drawings_heavy_vertical);

    for (const auto &column : columns) {
      auto field = column.RenderTitle();
      assert(field.GetSize() == common::coordinate_size_t(column.GetWidth(), 1));

      row.InsertMatrix(field);

      row.AppendChar(symbols::box_drawings_heavy_vertical);
    }

    return row;
  }
  [[nodiscard]] game::visual::ColoredCharMatrix MakeDivider(bool is_first, bool is_last) const {
    assert(!is_first || !is_last);

    ColoredCharMatrix row(common::coordinate_size_t(CalculateTotalWidth(), 1));

    if (is_first) {
      row.AppendChar(symbols::box_drawings_heavy_down_and_right);
    } else if (is_last) {
      row.AppendChar(symbols::box_drawings_heavy_up_and_right);
    } else {
      row.AppendChar(symbols::box_drawings_heavy_vertical_and_right);
    }

    for (const auto &column : columns) {
      for (int i = 0; i < column.GetWidth(); ++i) {
        row.AppendChar(symbols::box_drawings_heavy_horizontal);
      }

      if (is_first) {
        row.AppendChar(symbols::box_drawings_heavy_horizontal_and_down);
      } else if (is_last) {
        row.AppendChar(symbols::box_drawings_heavy_horizontal_and_up);
      } else {
        row.AppendChar(symbols::box_drawings_heavy_vertical_and_horizontal);
      }
    }

    common::Position last_position(row.GetSize().x - 1, 0);

    if (is_first) {
      row.SetChar(symbols::box_drawings_heavy_down_and_left, last_position);
    } else if (is_last) {
      row.SetChar(symbols::box_drawings_heavy_up_and_left, last_position);
    } else {
      row.SetChar(symbols::box_drawings_heavy_vertical_and_left, last_position);
    }

    return row;
  }

  /// Defines the layout of the table.
  std::list<TableColumn> columns{};

  friend TableRow;
};

}  // namespace game::visual

#endif  // CROSSING_CONSOLES_TABLE_H
