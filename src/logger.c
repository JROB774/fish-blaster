#define LOG_FILE_NAME "error.log"
#define LOG_BUFFER_SIZE 1024

GLOBAL FILE* gLogFile;

INTERNAL void log_message (const char* file, int line, LogLevel level, const char* msg_format, ...)
{
    // We only open the log file for writiing when the first call occurs.
    // This stops the file being created even if there's nothing to log.
    if (!gLogFile) gLogFile = fopen(LOG_FILE_NAME, "a");

    char time_buffer[LOG_BUFFER_SIZE] = {0}; // Buffer for storing the formatted curr time.
    char msg_buffer [LOG_BUFFER_SIZE] = {0}; // Buffer for storing the formatted log message.
    char line_buffer[LOG_BUFFER_SIZE] = {0}; // Buffer for storing the formatted log line.

    va_list args;
    va_start(args, msg_format);
    vsnprintf(msg_buffer, LOG_BUFFER_SIZE, msg_format, args);
    va_end(args);

    time_t rawtime = time(NULL);
    strftime(time_buffer, LOG_BUFFER_SIZE, "%m/%d/%Y %H:%M:%S", localtime(&rawtime));

    snprintf(line_buffer, LOG_BUFFER_SIZE, "[%s] Error in %s at line %d: %s\n",
        time_buffer, file, line, msg_buffer);

    if (gLogFile) fprintf(gLogFile, "%s", line_buffer);

    #if defined(BUILD_DEBUG)
    fprintf(stderr, "%s", line_buffer);
    #endif

    // Warning and above level messages also get displayed to the user in a dialog box.
    if (level >= LOG_LEVEL_WARNING)
    {
        U32 flags = (level == LOG_LEVEL_WARNING) ? SDL_MESSAGEBOX_WARNING : SDL_MESSAGEBOX_ERROR;
        SDL_ShowSimpleMessageBox(flags, "Error", msg_buffer, gWindow.window);

        // Error level messages also terminate the application when they occur.
        if (level == LOG_LEVEL_ERROR)
        {
            gWindow.running = false;
        }
    }
}

INTERNAL void quit_logger ()
{
    if (!gLogFile) return;
    fclose(gLogFile);
    gLogFile = NULL;
}
