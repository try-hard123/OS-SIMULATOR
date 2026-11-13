#include "File.h"

static Directory *
FindChild (Directory *current, const string &name)
{
  Directory *temp = current->child;

  while (temp != nullptr)
    {
      if (temp->name == name)
        return temp;
      temp = temp->sibling;
    }
  return nullptr;
}

static void
Unlink (Directory *current, Directory *node)
{
  if (current->child == node)
    current->child = node->sibling;
  else
    {
      Directory *prev = current->child;

      while (prev->sibling != node)
        prev = prev->sibling;

      prev->sibling = node->sibling;
    }

  delete node;
}

bool
TouchFile (Directory *current, const string &name)
{
  if (name == "." || name == "..")
    {
      cout << "touch: invalid file name '" << name << "'\n";
      return false;
    }

  Directory *old = FindChild (current, name);

  if (old != nullptr)
    {
      if (!old->isFile)
        {
          cout << "touch: cannot touch '" << name << "': Is a directory\n";
          return false;
        }
      return true; 
    }

  Directory *create = new Directory;
  Directory_init (create);

  create->name = name;
  create->parent = current;
  create->isFile = true;
  create->content = "";

  if (current->child == nullptr)
    current->child = create;
  else
    {
      Directory *temp = current->child;

      while (temp->sibling != nullptr)
        temp = temp->sibling;

      temp->sibling = create;
    }

  return true;
}

bool
WriteToFile (Directory *current, const string &name, const string &text)
{
  Directory *file = FindChild (current, name);

  if (file == nullptr)
    {
      cout << "write: " << name << ": No such file\n";
      return false;
    }

  if (!file->isFile)
    {
      cout << "write: " << name << ": Is a directory\n";
      return false;
    }

  file->content = text;
  return true;
}

bool
CatFile (Directory *current, const string &name)
{
  Directory *file = FindChild (current, name);

  if (file == nullptr)
    {
      cout << "cat: " << name << ": No such file or directory\n";
      return false;
    }

  if (!file->isFile)
    {
      cout << "cat: " << name << ": Is a directory\n";
      return false;
    }

  cout << file->content << "\n";
  return true;
}

bool
RemoveFile (Directory *current, const string &name)
{
  Directory *file = FindChild (current, name);

  if (file == nullptr)
    {
      cout << "rm: cannot remove '" << name
           << "': No such file or directory\n";
      return false;
    }

  if (!file->isFile)
    {
      cout << "rm: cannot remove '" << name << "': Is a directory\n";
      return false;
    }

  Unlink (current, file);
  return true;
}

bool
RemoveDir (Directory *current, const string &name)
{
  Directory *dir = FindChild (current, name);

  if (dir == nullptr)
    {
      cout << "rmdir: failed to remove '" << name
           << "': No such file or directory\n";
      return false;
    }

  if (dir->isFile)
    {
      cout << "rmdir: failed to remove '" << name << "': Not a directory\n";
      return false;
    }

  if (dir->child != nullptr)
    {
      cout << "rmdir: failed to remove '" << name
           << "': Directory not empty\n";
      return false;
    }

  Unlink (current, dir);
  return true;
}

bool
FileCommands (const string &cmd, vector<string> &argument, Directory *current)
{
  if (cmd == "touch")
    {
      if (argument.size () == 0)
        {
          cout << "touch: missing file operand\n";
          return true;
        }
      for (string &x : argument)
        TouchFile (current, x);
      return true;
    }

  else if (cmd == "write")
    {
      if (argument.size () < 2)
        {
          cout << "write: usage: write [file] [text]\n";
          return true;
        }

      string text = argument[1];
      for (int i = 2; i < (int) argument.size (); i++)
        text = text + " " + argument[i];

      WriteToFile (current, argument[0], text);
      return true;
    }

  else if (cmd == "cat")
    {
      if (argument.size () == 0)
        {
          cout << "cat: missing file operand\n";
          return true;
        }
      for (string &x : argument)
        CatFile (current, x);
      return true;
    }

  else if (cmd == "rm")
    {
      if (argument.size () == 0)
        {
          cout << "rm: missing operand\n";
          return true;
        }
      for (string &x : argument)
        RemoveFile (current, x);
      return true;
    }

  else if (cmd == "rmdir")
    {
      if (argument.size () == 0)
        {
          cout << "rmdir: missing operand\n";
          return true;
        }
      for (string &x : argument)
        RemoveDir (current, x);
      return true;
    }

  return false;
}