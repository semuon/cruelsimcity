#include "common_option_parser.h"

static void common_option_parser_CheckParameters();

void common_option_parser_Parse(int argc, char **argv, const std::string &app_name)
{
  try
  {
    TCLAP::CmdLine cmd(app_name.c_str(), ' ', "1.0");

    ArgProvidedVisitor argSeedVisitor;
    TCLAP::ValueArg<uint> argSeed("", "seed", "Seed for random number generator. By default current time is used.", false, 0, "path", cmd, &argSeedVisitor);

    TCLAP::ValueArg<std::string> argPrefix("", "prefix", "Prefix which will appear in all filenames: prefix.filename.", false, "", "prefix", cmd);
    TCLAP::ValueArg<int> argNthreads("", "n-threads", "Number of OpenMP threads. By default is 1. \
                                                       Use argument 0 in order to determine the value automatically (maximal available).", 
                                                      false, 1, "n", cmd);

    TCLAP::SwitchArg switchOverwriteLogs("", "append-logs", "Append log files if they exist", cmd, true);
    TCLAP::ValueArg<std::string> argLogsDir("", "logs-dir", "Directory for log files. By default stdout and stderr are used.", false, "", "path", cmd);
    TCLAP::ValueArg<std::string> argDataDir("", "data-dir", "Directory for data files. By default current directory is used.", false, ".", "path", cmd);

    try
    {
      option_parser_Parse(cmd, argc, argv);
    }
    catch(WrongCmdLineOption &ex)
    {
      std::cerr << "Wrong option value: " << ex.what() << std::endl;
      std::exit(-1);
    }

    pLogsDirPath = argLogsDir.getValue();
    pDataDirPath = argDataDir.getValue();
    pOverwriteLogs = switchOverwriteLogs.getValue();
    
    pNthreads = argNthreads.getValue();
    pPrefix = argPrefix.getValue();
    
    pSetSeed = argSeedVisitor.IsFlagSet();
    if (pSetSeed) pSeed = argSeed.getValue();

    try
    {
      common_option_parser_CheckParameters();
    }
    catch(WrongCmdLineOption &ex)
    {
      std::cerr << "Wrong option value: " << ex.what() << std::endl;
      std::exit(-1);
    }

  }
  catch (TCLAP::ArgException &e)
  {
    std::cerr << "Option error: " << e.error() << " for " << e.argId() << std::endl;
    std::exit(-1);
  }
}

static void
common_option_parser_CheckParameters()
{
}

void
common_option_parser_PrintParameters(int argc, char **argv)
{
  pStdLogs.Write("Command line:\n");

  for (int i = 0; i < argc; i++)
    pStdLogs.Write("%s ", argv[i]);

  pStdLogs.Write("\n\n");

  common_option_parser_CheckParameters();

  if (!pPrefix.empty())
  {
    pStdLogs.Write("Prefix:                                        %s\n", pPrefix.c_str());
  }
  if (!pLogsDirPath.empty())
  {
    pStdLogs.Write("Logs output directory:                         %s\n", pLogsDirPath.c_str());
    pStdLogs.Write("  Overwrite logs:                              %s\n", (pOverwriteLogs) ? "YES" : "NO");
  }

  pStdLogs.Write("Data output directory:                         %s\n", pDataDir.GetDirectory().c_str());

  if (pSetSeed)
    pStdLogs.Write("Seed for ranlxd:                              % -d\n", pSeed);
  else
    pStdLogs.Write("Seed for ranlxd (automatic):                  % -d\n", pSeed);

  pStdLogs.Write("\n");
  pStdLogs.Write("Run parameters:\n");
  
  option_parser_PrintParameters();
}
