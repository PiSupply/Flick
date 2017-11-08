#!/usr/bin/env python

import flicklib
import requests
from os import getenv
from time import sleep
from copy import copy

"""
This is a simple proof-of-concept  demonstration of how
you can use a Flick with Screenly OSE (https://www.screenly.io/ose).

We listen for movements in the x-axis and move to the next
or previous slide using.
"""

# You can override this by exporting the
# SCREENLY_OSE_API and SCREENLY_OSE_ENDPOINT
# environment variables.

SCREENLY_OSE_API = getenv(
    'SCREENLY_OSE_API',
    'http://localhost:8080'
)
SCREENLY_OSE_ENDPOINT = getenv(
    'SCREENLY_OSE_ENDPOINT',
    'api/v1/assets/control'
)


def switch_slide(action):
    if action not in ['next', 'previous']:
        print('{} is an invalid action.'.format(action))
        return False

    url = '{}/{}/{}'.format(
        SCREENLY_OSE_API,
        SCREENLY_OSE_ENDPOINT,
        action
    )

    r = requests.get(url)
    print('Got action {} (status code: {})'.format(action, r.status_code))
    if r.ok:
        sleep(0.5)
    return


@flicklib.move()
def move(x, y, z):
    global xyz
    xyz = [x, y, z]


def main():
    global xyz
    xyz = [0, 0, 0]
    old_xyz = [0, 0, 0]

    while True:
        if xyz[0] > old_xyz[0]:
            switch_slide('next')
        elif xyz[0] < old_xyz[0]:
            switch_slide('previous')
        old_xyz = copy(xyz)
        sleep(0.3)


if __name__ == "__main__":
    main()
