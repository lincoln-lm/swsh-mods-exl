#!/usr/bin/env python3

import ftplib
import os
from types import NoneType
from typing import Any, Dict, Optional, Self, Tuple, Union

# https://ftputil.sschwarzer.net
import ftputil


# Wrapper to assert the enviroment variable exists.
def getenv(
    key: str, default: Optional[str] = None, *args: Tuple[Any], **kwargs: Dict[str, Any]
) -> str:
    """Wrapper for os.getenv."""

    value: Union[None, str] = os.getenv(key)
    # Use the value if it exists.
    if value is not None:
        return value
    # If we're provided a default, fallback to that.
    elif default is not None:
        return default
    # Assert key is missing.
    else:
        raise KeyError(
            f"Missing enviroment variable {key}! Only run this script via make deploy-ftp."
        )


# Get enviroment variables from build system.
PROGRAM_ID: str = getenv("PROGRAM_ID")
FTP_IP: str = getenv("FTP_IP")
FTP_PORT: int = int(getenv("FTP_PORT"))
FTP_USERNAME: str = getenv("FTP_USERNAME")
OUT: str = getenv("OUT")
SD_OUT: str = getenv("SD_OUT")
# Password may be empty, so special case this.
FTP_PASSWORD: str = getenv("FTP_PASSWORD", "")


class SessionFactory(ftplib.FTP):
    """Session factory for FTPHost."""

    def __init__(
        self: Self,
        ftp_ip: str,
        ftp_port: int,
        ftp_username: str,
        ftp_password: str,
        *args: Tuple[Any],
        **kwargs: Dict[str, Any],
    ) -> NoneType:
        super().__init__()

        # Connect to FTP server.
        self.connect(ftp_ip, ftp_port)
        # Login with credentials.
        self.login(ftp_username, ftp_password)


def main(*args: Tuple[Any], **kwargs: Dict[str, Any]) -> NoneType:
    # Connect/login to console FTP server.
    with ftputil.FTPHost(
        FTP_IP, FTP_PORT, FTP_USERNAME, FTP_PASSWORD, session_factory=SessionFactory
    ) as ftp_host:
        # Make output directory.
        ftp_host.makedirs(SD_OUT, exist_ok=True)
        ftp_host.makedirs("config/swsh-mods-exl/", exist_ok=True)
        # Upload config file.
        ftp_host.upload(
            os.path.join(os.curdir, "config.toml"),
            ftp_host.path.join("config/swsh-mods-exl/", "config.toml"),
        )
        def upload_directory(sub_directory: str) -> NoneType:
            directory = os.path.join(OUT, sub_directory)
            ftp_directory = os.path.join(SD_OUT, sub_directory)
            ftp_host.makedirs(ftp_directory, exist_ok=True)
            for name in os.listdir(directory):
                if not os.path.isfile(os.path.join(directory, name)):
                    upload_directory(os.path.join(sub_directory, name))
                else:
                    ftp_host.upload(
                        os.path.join(directory, name),
                        ftp_host.path.join(ftp_directory, name)
                    )
        upload_directory("")


if __name__ == "__main__":
    main()
