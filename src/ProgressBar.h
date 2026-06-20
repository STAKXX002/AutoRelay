#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <string>
#include <cstddef>

class ProgressBar {
public:
    // Enables ANSI/VT escape-sequence processing on Windows consoles that
    // don't support it by default (legacy cmd.exe). Safe no-op everywhere
    // else, including Windows Terminal, which already supports it.
    static void enableAnsiSupport();

    // Renders/updates the progress bar with the current file's name shown
    // underneath, in place (no scrolling). If stdout isn't an interactive
    // terminal (piped to a file, redirected into a log, run in CI), falls
    // back to one plain "[n/total] filename" line per call instead, so
    // redirected output doesn't fill up with escape-sequence noise.
    static void update(std::size_t current, std::size_t total, const std::string& currentFile);

    // Erases the rendered bar (if any) so the next thing printed -- e.g. an
    // error message -- doesn't get overwritten by the following update().
    static void clear();

    // Call once after the last update() so subsequent normal output starts
    // on a fresh line below the bar instead of overlapping it.
    static void finish();

private:
    static bool isRendered;
};

#endif // PROGRESSBAR_H
