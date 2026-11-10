#include "CommandParser.h"
#include "ProcessManagement.h"

void
AvilableCommands ()
{
  cout << "Available Commands:\n";
  cout << "mkdir <argument>\n";
  cout << "cd <argument>\n";
  cout << "ls\n";
  cout << "pwd\n";
  cout << "newproc\n";
  cout << "list\n";
  cout << "ps\n";
  cout << "kill\n";
  cout << "info\n";
  cout << "state\n";
  return;
}

typedef enum{ PROCESS_CMD_OK, PROCESS_CMD_ERR}IS_PROCESS_CMD;


IS_PROCESS_CMD isprocess(string cmd){
  if(cmd == "newproc" || cmd == "list" || cmd == "ps" || cmd == "kill" || cmd == "info" || cmd == "state"){
    return PROCESS_CMD_OK;
  }

  return PROCESS_CMD_ERR;
}

bool
CommandParser::execute (string command)
{
  stringstream ss (command);
  string cmd;
  ss >> cmd;

  if (cmd.empty()) return true;

  vector<string> argument;
  string arg;
  while (ss >> arg)
    {
      argument.push_back (arg);
    }
  if (cmd == "help")
    {
      AvilableCommands ();
      return true;
    }
  else if (cmd == "mkdir")
    {
      if (argument.size () == 0)
        {
          cout << "mkdir: missing operand\n";
          cout << "Try 'mkdir --help' for more information.\n";
          return true;
        }
      for ( string &x : argument)
        {
          if (fs->mkdir (x)){
            continue;
          }
          else
            {
              cout << "ERROR : DIRECTORY ALREADY EXISTS\n";
              return true;
            }
        }
      return true;
    }
  else if (cmd == "ls")
    {
      if (argument.size () != 0)
        {
          for(int i = 0; i < argument.size(); i++){
             cout << "ls: cannot access '";
             cout << argument[i];
             cout << "': No such file or directory\n";
          }
          return true;
        }
      else{
          fs->ls();
        }
      return true;
    }
  else if (cmd == "pwd")
    {
      fs->pwd();
      return true;
    }
  else if (cmd == "cd")
    {
    if (argument.size () > 1)
        {
          cout << "bash: cd: too many arguments\n";
          return true;
        }
    else{
        fs->cd(argument[0]);
      }
      return true;
    }
  else if (!isprocess(cmd)){
      ProcessManagement(cmd, argument);
      return true;
    }
  else if (cmd == "exit")
    {
      return false;
    }
  else
    {
      cout << "bash: " << cmd << ": command not found\n";
      return true;
    }
}
