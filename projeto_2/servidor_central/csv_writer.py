import csv
from datetime import datetime


class CSV:
    def __init__(self):
        filename = 'log.csv'
        self.file = open(filename, 'w')

        fields = ['Date/Time', 'Event']
        self._writer = csv.DictWriter(self.file, fieldsnames=fields)
        self._writer.writeheader()

    def write(self, raw_event):
        if raw_event[0] == 'T':
            event = f'Toggled {raw_event[1:].upper()}'
        elif 'temperature' in raw_event:
            event = 'Updated Temperature/Humidity'
        else:
            event = 'Updated Switches/Sensors state'

        row = dict()
        row['Date/Time'] = str(datetime.now())
        row['Event'] = event
        self._writer.writerow(row)

    def close(self):
        self.file.close()
        print('Log file closed')
