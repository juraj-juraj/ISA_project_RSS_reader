#pragma once

constexpr const char *FEEDFILE_ARG = "-f";
constexpr const char *CERTFILE_ARG = "-c";
constexpr const char *CERTDIR_ARG = "-C";
constexpr const char *DATESHOW_ARG = "-T";
constexpr const char *AUTHOR_ARG = "-a";
constexpr const char *URLSHOW_ARG = "-u";
constexpr const char *URLPOS_ARG = "URL";
constexpr const char *HELP_ARG = "-h";

constexpr const char *HELP_MSG =
        "The all new feed downloader version 1.1 alpha\n"
        "Usage:\n"
        "   feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]\n"
        "\n"
        "Arguments:\n"
        "   -f                  Specify feedfile from which URL will be taken from\n"
        "   -c                  Specify certificate to use when checking server certificate\n"
        "   -C                  Specify directory with certificates\n"
        "   -T                  Print date of last change to item in feed\n"
        "   -a                  Print author of item in feed\n"
        "   -u                  Print URL to item in feed\n"
        "\n"
        "Useful tool to download and parse feed in format RSS or atom\n";
