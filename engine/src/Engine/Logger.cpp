#include "Logger.hpp"
Logging::StdOutLogger::StdOutLogger(const Logging::LoggingConfig_t& config) :
  LoggerBase(config), levels(config.find("color") != config.end() ? colored : uncolored)
{
}

void Logging::StdOutLogger::log(const std::string& message, Logging::LogLevel level)
{
  if (!logable(level))
  {
    return;
  }

  std::string output;
  output.reserve(message.length() + 64);
  output.append(timestamp());
  output.append(levels.find(level)->second);
  output.append(message);
  output.push_back('\n');
  log(output);
}

void Logging::StdOutLogger::log(const std::string& message)
{
  std::cout << message;
  std::cout.flush();
}

Logging::LoggerBase* Logging::LoggerFactory::produce(const Logging::LoggingConfig_t& config) const
{
  // grab the type
  auto type = config.find("type");
  if (type == config.end())
  {
    throw std::runtime_error("Logging factory configuration requires a type of logger");
  }

  // grab the logger
  auto found = find(type->second);
  if (found != end())
  {
    return found->second(config);
  }

  // couldn't get a logger
  throw std::runtime_error("Couldn't produce logger for type: " + type->second);
}

Logging::FileLogger::FileLogger(const Logging::LoggingConfig_t& config) : LoggerBase(config)
{
  // grab the file name
  auto name = config.find("file_name");
  if (name == config.end())
  {
    throw std::runtime_error("No output file provided to file logger");
  }

  file_name = name->second;

  // if we specify an interval
  reopen_interval = std::chrono::seconds(300);
  auto interval   = config.find("reopen_interval");
  if (interval != config.end())
  {
    try
    {
      reopen_interval = std::chrono::seconds(std::stoul(interval->second));
    }
    catch (...)
    {
      throw std::runtime_error(interval->second + " is not a valid reopen interval");
    }
  }

  reopen();
}

void Logging::FileLogger::log(const std::string& message, const Logging::LogLevel level)
{
  if (!logable(level))
  {
    return;
  }

  std::string output;
  output.reserve(message.length() + 64);
  output.append(timestamp());
  output.append(uncolored.find(level)->second);
  output.append(message);
  output.push_back('\n');
  log(output);
}

void Logging::FileLogger::log(const std::string& message)
{
  lock.lock();
  file << message;
  file.flush();
  lock.unlock();
  reopen();
}

void Logging::FileLogger::reopen()
{
  // check if it should be closed and reopened
  auto now = std::chrono::system_clock::now();
  lock.lock();
  if (now - last_reopen > reopen_interval)
  {
    last_reopen = now;
    try
    {
      file.close();
    }
    catch (...)
    {
    }
    try
    {
      file.open(file_name, std::ofstream::out | std::ofstream::app);
      last_reopen = std::chrono::system_clock::now();
    }
    catch (std::exception& e)
    {
      try
      {
        file.close();
      }
      catch (...)
      {
      }
      throw e;
    }
  }
  lock.unlock();
}
