#pragma once

#include <string>
#include <unistd.h>

class OsOp {
public:
  auto operator<=>(const OsOp &rhs) const = default;

  enum FileOp { Append, Truncate };

  OsOp(std::string plot_script, std::string data_file)
      : plot_script{plot_script}, data_file{data_file} {}

  static void write_to_file(std::string file, std::string content,
                            FileOp mode = FileOp::Truncate);

  /**
   * Store the current Catalan structures into a file on the disk
   *
   * @param file:  output file name
   */
  virtual void to_file(FileOp mode = FileOp::Truncate, std::string file = "") = 0;

  /**
   * Save the plot of the current Catalan structures into a svg file
   *
   * @param outfile:            The svg filename
   */
  void to_svg(std::string outfile);

  /**
   * Plot the current Catalan structure. A hidden file will be created and
   * the plot will take over the window focus.
   */
  void plot();

  /**
   * Plot the current Catalan structure. No hidden file will be created
   *
   * WARNING: This is probably not what you want. Think before calling it.
   */
  void _just_plot(std::string outfile);

private:
  std::string plot_script;
  std::string data_file;
};
