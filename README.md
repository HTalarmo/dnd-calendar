# dnd-calendar
This is a simple calendar application to use to assist with GMing for Dungeons & Dragons and other tabletop RPGs. It still looks pretty rough on a few spots (namely the timekeeping controls and dialogs) but is functional. Sessions can be saved and loaded in .save files which store the data in .json format.

## Timekeeping
The calendar allows the GM to keep track of time during a game by allowing them to set a date and time for the current time and controls to change it. The GUI comes with 4 pre-programmed buttons for +1 turn (6 seconds), +10 minutes, +1 hour and Rest, which calculates the amount of hours required for a group of n partymembers to rest with equal watches taken over the course of the night so that all partymembers get full 8 hours of sleep.

The calendar also comes with a stopwatch that can be used to track time in real time if you want to run time in real time for RP reasons. The stopwatch can also be set to run in speeds faster than real life. For example it can be made to tick 2 seconds forward for every 1 second that passes in real life. At the moment it only ticks forward on every second and doesn't just simply tick faster. The time from the stopwatch can when desired then be either cleared out or added to the current time.

The calendar also allows custom time addition with simple year/day/hour/minute/second controls that allows the GM to add or remove any arbitrary amount of time to the current time.

## Event listing
The calendar has a simple event list included to which the GM can add notable events that happen during adventuring. They are kept in order at all times with the newest at the top and oldest at the bottom. The GM can add and remove events but while the GUI currently has buttons for "Save Events" and "Load Events" they do nothing as the save and load are integrated with session saving and loading.

## Lunar cycles
The calendar also allows the GM to keep track of the lunar cycles if desired. It will show the current lunar phase as well as how many days away the next full moon and new moon are for each moon. The moons can be set up in the custom calendars.

## Custom calendars
Calendars are stored in a .json file called [calendars.json](https://github.com/HTalarmo/dnd-calendar/blob/master/calendars.json). The structure of the file can be described as follows:
```
"calendars":[
  {
    "name":"calendar name,
    "months":[
      list of month names
    ],
    "days_of_week":[
      list of the names of each day of week (optional)
    ],
    "month_lengths":[
      integer lengths of each month listed in the same order as the months appeared. IMPORTANT: they must add up to the same number as "days_in_year" or the calendar will be invalid
    ],
    "start_day": integer offset day to offset different calendars,
    "days_in_year": integer value of how many days there are in a year 
  }
],
"moons":[
  {
    "name": Name of the moon,
    "orbital_period": integer, how many days does a single orbit take for the moon,
    "start_date": integer offset day for the moon orbit,
    "rotation": rotates the moon by given integer of degrees on the GUI. (currently slightly bugged, rotation also causes the drawn moon to be moved a little),
    "color":{ color of the moon
      "red:" int,
      "green": int,
      "blue": int
    }
  }
]
```

The program can handle calendars having a varying amount of days in their years but if the amount of days in all the months combined doesn't add up to the days in the year, it will simply invalidate the calendar. 
