#include <string>
#include "Buffer.h"

class Editor {
private:
  Buffer *buffer;
  int currentLine;

  void dispatchCmd(const string &cmd);
  void cmdAppend();
  void cmdInsert();
  void cmdDelete(int start, int end);
  void cmdNumber(int start, int end);
  void cmdWrite(const string &filename);
  void cmdNull(int line);
public:
  Editor();
  ~Editor();
  void run();
};
