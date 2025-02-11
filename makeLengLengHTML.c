#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char htmlStarting[] = "<!DOCTYPE html>\n<meta charset=\"utf-8\" />\n<!-- version 1 -->\n<title>Gameplay replay</title>\n<style>\nhtml,body {font-family:Verdana, sans-serif;font-size:10pt;margin:0;padding:0;}body{padding:12px 0;} .battle-log {font-family:Verdana, sans-serif;font-size:10pt;} .battle-log-inline {border:1px solid #AAAAAA;background:#EEF2F5;color:black;max-width:640px;margin:0 auto 80px;padding-bottom:5px;} .battle-log .inner {padding:4px 8px 0px 8px;} .battle-log .inner-preempt {padding:0 8px 4px 8px;} .battle-log .inner-after {margin-top:0.5em;} .battle-log h2 {margin:0.5em -8px;padding:4px 8px;border:1px solid #AAAAAA;background:#E0E7EA;border-left:0;border-right:0;font-family:Verdana, sans-serif;font-size:13pt;} .battle-log .chat {vertical-align:middle;padding:3px 0 3px 0;font-size:8pt;} .battle-log .chat strong {color:#40576A;} .battle-log .chat em {padding:1px 4px 1px 3px;color:#000000;font-style:normal;} .chat.mine {background:rgba(0,0,0,0.05);margin-left:-8px;margin-right:-8px;padding-left:8px;padding-right:8px;} .spoiler {color:#BBBBBB;background:#BBBBBB;padding:0px 3px;} .spoiler:hover, .spoiler:active, .spoiler-shown {color:#000000;background:#E2E2E2;padding:0px 3px;} .spoiler a {color:#BBBBBB;} .spoiler:hover a, .spoiler:active a, .spoiler-shown a {color:#2288CC;} .chat code, .chat .spoiler:hover code, .chat .spoiler:active code, .chat .spoiler-shown code {border:1px solid #C0C0C0;background:#EEEEEE;color:black;padding:0 2px;} .chat .spoiler code {border:1px solid #CCCCCC;background:#CCCCCC;color:#CCCCCC;} .battle-log .rated {padding:3px 4px;} .battle-log .rated strong {color:white;background:#89A;padding:1px 4px;border-radius:4px;} .spacer {margin-top:0.5em;} .message-announce {background:#6688AA;color:white;padding:1px 4px 2px;} .message-announce a, .broadcast-green a, .broadcast-blue a, .broadcast-red a {color:#DDEEFF;} .broadcast-green {background-color:#559955;color:white;padding:2px 4px;} .broadcast-blue {background-color:#6688AA;color:white;padding:2px 4px;} .infobox {border:1px solid #6688AA;padding:2px 4px;} .infobox-limited {max-height:200px;overflow:auto;overflow-x:hidden;} .broadcast-red {background-color:#AA5544;color:white;padding:2px 4px;} .message-learn-canlearn {font-weight:bold;color:#228822;text-decoration:underline;} .message-learn-cannotlearn {font-weight:bold;color:#CC2222;text-decoration:underline;} .message-effect-weak {font-weight:bold;color:#CC2222;} .message-effect-resist {font-weight:bold;color:#6688AA;} .message-effect-immune {font-weight:bold;color:#666666;} .message-learn-list {margin-top:0;margin-bottom:0;} .message-throttle-notice, .message-error {color:#992222;} .message-overflow, .chat small.message-overflow {font-size:0pt;} .message-overflow::before {font-size:9pt;content:'...';} .subtle {color:#3A4A66;}\n</style>\n<div class=\"wrapper replay-wrapper\" style=\"max-width:1180px;margin:0 auto\">\n<input type=\"hidden\" name=\"replayid\" value=\"\" />\n<div class=\"battle\"></div><div class=\"battle-log\"></div><div class=\"replay-controls\"></div><div class=\"replay-controls-2\"></div>\n<h1 style=\"font-weight:normal;text-align:center\"><br />(i am too lazy to do this part) vs. (so is this part)</h1>\n<script type=\"text/plain\" class=\"battle-log-data\">\n";
char htmlEnding[] = "\n\n</script>\n</div>\n</div>\n<div class=\"battle-log battle-log-inline\"><div class=\"inner\">So is the log of the battle. Enjoy!</div></div>\n</div>\n\n<script>\nlet daily = Math.floor(Date.now()/1000/60/60/24);document.write('<script src=\"https://play.pokemonshowdown.com/js/replay-embed.js?version'+daily+'\"></'+'script>');\n</script>\n";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./makeLengLenghtml filename\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    int positionOfDot = 0;
    for (; filename[positionOfDot] != '.'; positionOfDot++)
        ;
    char *htmlFilename = malloc(positionOfDot + 5);
    strncpy(htmlFilename, filename, positionOfDot);
    strcpy(htmlFilename + positionOfDot, ".html");
    FILE *html = fopen(htmlFilename, "w");
    free(htmlFilename);

    fprintf(html, "%s", htmlStarting);

    bool fstcase = false;
    bool sndcase = false;
    bool trdcase = false;
    while (fstcase == false && sndcase == false && trdcase == false)
    {
        char c = fgetc(file);
        if (c == EOF)
        {
            printf("Error: file does not contain the necessary information\n");
            return 1;
        }
        if (fstcase == false)
        {
            if (c == '|')
            {
                fstcase = true;
            }
        }
        else if (sndcase == false)
        {
            if (c == 'j')
            {
                sndcase = true;
            }
            else
            {
                fstcase = false;
            }
        }
        else if (trdcase == false)
        {
            if (c == '|')
            {
                trdcase = true;
            }
            else
            {
                fstcase = false;
                sndcase = false;
            }
        }
    }
    fprintf(html, "%c", '|');

    while (true)
    {
        char c = fgetc(file);
        if (c == EOF)
        {
            break;
        }
        fprintf(html, "%c", c);
    }
    fclose(file);

    fprintf(html, "%s", htmlEnding);

    fclose(html);
    return 0;
}
