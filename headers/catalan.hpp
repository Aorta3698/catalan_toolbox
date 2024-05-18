#pragma once

class Catalan {
  virtual void plot() = 0;
  virtual Catalan *next() = 0;
  virtual Catalan *to_tree() = 0;
  virtual void test() = 0;
};
