#include <string>
#include <unistd.h>

class OsOp {
public:
  auto operator<=>(const OsOp &rhs) const = default;

  OsOp(std::string plot_script, std::string data_file)
      : plot_script{plot_script}, data_file{data_file} {}

  /**
   * Store the current Catalan structures into a file on the disk
   *
   * @param file:  output file name
   */
  virtual void to_file(std::string file = "") = 0;

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

private:
  std::string plot_script;
  std::string data_file;
};
