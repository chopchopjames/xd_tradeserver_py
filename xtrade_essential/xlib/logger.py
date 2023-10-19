## coding: utf8

# PyAlgoTrade
#
# Copyright 2011-2015 Gabriel Martin Becedillas Ruiz
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# import sys
# import codecs
# sys.stdout = codecs.getwriter("utf-8")(sys.stdout.detach())
# celery worker里面会报错

import logging
import threading

initLock = threading.Lock()
rootLoggerInitialized = False

format = "||%(asctime)s||%(name)s||%(levelname)s||%(module)s||%(funcName)s||%(lineno)s||%(message)s||"


def getLogger(name=None, level=logging.INFO):
    global rootLoggerInitialized
    with initLock:
        if not rootLoggerInitialized:
            logger = logging.getLogger(name)
            logger.setLevel(level)

            # remove其他handler，避免出现重复打印的问题
            for handler in logger.handlers[:]:
                logger.removeHandler(handler)

            handler = logging.StreamHandler()
            handler.setFormatter(logging.Formatter(format))
            logger.addHandler(handler)

            rootLoggerInitialized = True

            return logger

    return logging.getLogger(name)
