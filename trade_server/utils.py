# -*- coding: utf-8 -*-
"""
.. moduleauthor:: Zhixiong Ge<56582881@qq.com>
"""

import pytz
import time
import random
import functools
import traceback

from datetime import datetime


CN_TZ = pytz.timezone('Asia/Shanghai')


def catch_exceptions(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            tb_str = traceback.format_exception(etype=type(e), value=e, tb=e.__traceback__)
            print(f"Exception occurred in {func.__name__}: {e}")
            print("".join(tb_str))

    return wrapper


def generate_uid() -> str:
    """ 订单号不能超过30位

    :return:
    """
    timestamp = int(time.time())
    random_num = random.randint(1, 999999)
    uid = f"{timestamp}{random_num:06}"  # Ensure random_num is 6 digits
    return uid[:30]



def parseDatetimeStr(dt_str: str):
    return CN_TZ.localize(datetime.strptime(dt_str, '%Y%m%d%H%M%S%f'))

