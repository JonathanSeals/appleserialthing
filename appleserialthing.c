//
//  appleserialthing.c
//  appleserialthing
//
//  thing for getting the manufacture week/year from an apple serial
//
//  shoutout to this guy: https://gist.github.com/pudquick
//  for this: https://gist.github.com/pudquick/05e877faee1ad5059690
//
//  Created by notjon on 10/12/17.
//  Copyright © 2017 notjon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, const char * argv[]) {
    if (!argv[1]) {
        printf("missing arg\n");
        return -1;
    }
    
    if (!strlen(argv[1])) {
        printf("missing arg\n");
        return -1;
    }
    
    if (strlen(argv[1]) < 11 || strlen(argv[1]) > 12) {
        printf("invalid serial length\n");
        return -1;
    }
    
    if (strlen(argv[1]) == 11) {
        if (atoi(argv[1]+2) && atoi(argv[1]+3) && atoi(argv[1]+4)) {
            
     		char week[3], year[2];
            
            memcpy(year, argv[1]+2, 0x1);
            memcpy(year+1, "\x00", 0x1);
            
            uint32_t yearint = atoi(year);
            
            /* This is lazy and probably wrong */
            
            if (!yearint)
                yearint=2010;
            else yearint=2000+yearint;
            
            memcpy(week, argv[1]+3, 0x2);
            memcpy(week+2, "\x00", 0x1);
            
            if (atoi(week) > 52) {
                printf("invalid serial\n");
                return -1;
            }
            
            printf("manufacture year: %u\nmanufacture week: %s\n", yearint, week);
            return 0;
        }
        else {
            printf("invalid serial\n");
            return -1;
        }
    }

    char *alpha_year = "cdfghjklmnpqrstvwxyz";
    
    char serial[0xD];
    
    memcpy(serial, argv[1], strlen(argv[1]));
    memcpy(serial+0xC, "\x00", 0x1);
    
    for (int a=0; a < 0xC; a++) {
        serial[a] = tolower(serial[a]);
    }
    
    char year[2];
    
    memcpy(year, serial+3, 0x1);
    memcpy(year+1, "\x00", 0x1);
    
    char week[2];
    
    memcpy(week, serial+4, 0x1);
    memcpy(week+1, "\x00", 0x1);
    
    int locOfYearByte = -1;
    
    for (int aq=0; aq < strlen(alpha_year); aq++) {
        if (!memcmp(&alpha_year[aq], year, 0x1)) {
            locOfYearByte = aq;
        }
    }
    
    char *alpha_week = " 123456789cdfghjklmnpqrtvwxy";
    
    int locOfWeekByte = -1;
    
    for (int aq=0; aq < strlen(alpha_week); aq++) {
        if (!memcmp(&alpha_week[aq], week, 0x1)) {
            locOfWeekByte = aq;
        }
    }
    
    uint32_t yearmaybe = -1;
    
    if (locOfYearByte == -1) {
        printf("didn't find year\n");
        return -1;
    }
    else {
        yearmaybe = (2010+(locOfYearByte/2));
    }
    
    if (yearmaybe == -1) {
        printf("err\n");
        return -1;
    }

    uint32_t weekmaybe = -1;
    
    if (locOfWeekByte == -1) {
        printf("didn't find week\n");
        return -1;
    }
    else {
        int estHalf = (locOfYearByte)%2;
        weekmaybe = (locOfWeekByte+(estHalf*26));
    }
    
    printf("manufacture year: %u\nmanufacture week: %u\n", yearmaybe, weekmaybe);
    return 0;
}
