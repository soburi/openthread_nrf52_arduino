import json
import sys
import os
import hashlib

with open(sys.argv[1], 'r') as jsn:
    pkgs = json.load(jsn)

    tag = sys.argv[2]
    sha = hashlib.sha256(open('release.zip', 'rb').read()).hexdigest()
    size = os.path.getsize('release.zip')

    node = {
        "name": "OpenThread nRF52840 Boards",
        "architecture": "otnrf52",
        "category": "Contributed",
        "version": tag,
        "url": f"https://github.com/soburi/openthread_nrf52_arduino/archive/{tag}.zip",
        "archiveFileName": f"openthread_nrf52_arduino-{tag}.zip",
        "checksum": f"SHA-256:{sha}",
        "size": f"{size}",
        "boards": [ {"name": "OpenThread nRF52840 boards"} ],
        "toolsDependencies": [ {
            "packager": "adafruit",
            "name": "arm-none-eabi-gcc",
            "version": "9-2019q4"
        }, {
            "packager": "adafruit",
            "name": "arm-none-eabi-gcc",
            "version": "9.4.0"
        } ]
    }

    pkgs["packages"][0]["platforms"].insert(0, node)

    print(json.dumps(pkgs, indent=2))
