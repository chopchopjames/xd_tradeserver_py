# -*- coding: utf-8 -*-
"""
.. moduleauthor:: Zhixiong Ge<56582881@qq.com>
"""

import typing
from datetime import datetime

from xd_wrapper import TradeApi as tdapi
from common.tradeHandler.async_server import AsyncBaseTradeServer, LimitOrder, EtfConvertRequest
from common.tradeHandler.data_models import AccountBalance, AccountHolding, QryOrderResult

from .utils import catch_exceptions, generate_uid, parseDatetimeStr


class TradeServer(tdapi.TradeSpi, AsyncBaseTradeServer):
    def __init__(self, hostname: str):
        tdapi.TradeSpi.__init__(self)
        AsyncBaseTradeServer.__init__(self, hostname=hostname)

        self.__req_id = 0

        self.__date_str = datetime.now().strftime("%Y%m%d")

        self.__holdings = dict()
        self.__qry_order_result = list()

        self.__start_canceled = False

        self.api = tdapi.TradeApi()
        self.api.RegisterSpi(self)
        self.login()

    def getReq(self):
        self.__req_id += 1
        return str(self.__req_id)

    @catch_exceptions
    def OnOrderReport(self, orderInfo: tdapi.OrderInfo):
        self.getLogger().info(f"order_id: {orderInfo.order_id}, symbol: {orderInfo.symbol}, "
              f"status: {orderInfo.status}, msgstr: {orderInfo.msgstr}, "
              f"ordertime: {orderInfo.ordertime}, filled_price: {orderInfo.filled_price}, "
              f"filled_qty: {orderInfo.filled_qty}， broker_id: {orderInfo.broker_id}")

        if orderInfo.status == 0:
            order = self.getOrderByCustId(orderInfo.order_id)
            self.onOrderAcceptedResp(
                exchange_order_ref=orderInfo.order_id,
                strategy_order_ref=order.getReqId(),
                accepted_time=parseDatetimeStr(self.__date_str+orderInfo.ordertime),
            )
            self.api.QueryOrder(self.getReq())

        elif orderInfo.status == 4: # 已撤
            order = self.getOrderByExchangeId(orderInfo.order_id)
            if order is not None:
                self.getLogger().info(f"cannot find order: {orderInfo.order_id}")
                return

            self.onExecInfo(
                exchange_order_ref=orderInfo.order_id,
                dateTime=parseDatetimeStr(self.__date_str+orderInfo.ordertime),
                fill_quantity=orderInfo.filled_qty,
                avg_fill_price=orderInfo.filled_price,
                cost=0,
                msg=orderInfo.msgstr,
            )

            self.onOrderCanceledResp(
                order=order,
                canceled_time=parseDatetimeStr(self.__date_str+orderInfo.ordertime),
            )

        elif orderInfo.status == 5: # 部分成交
            order = self.getOrderByExchangeId(orderInfo.order_id)
            if order is not None:
                self.getLogger().info(f"cannot find order: {orderInfo.order_id}")
                return

            self.onExecInfo(
                exchange_order_ref=orderInfo.order_id,
                dateTime=parseDatetimeStr(self.__date_str+orderInfo.ordertime),
                fill_quantity=orderInfo.filled_qty,
                avg_fill_price=orderInfo.filled_price,
                cost=0,
                msg=orderInfo.msgstr,
            )

        elif orderInfo.status == 6: # 全部成交
            order = self.getOrderByExchangeId(orderInfo.order_id)
            if order is not None:
                self.getLogger().info(f"cannot find order: {orderInfo.order_id}")
                return

            self.onExecInfo(
                exchange_order_ref=orderInfo.order_id,
                dateTime=parseDatetimeStr(self.__date_str+orderInfo.ordertime),
                fill_quantity=orderInfo.filled_qty,
                avg_fill_price=orderInfo.filled_price,
                cost=0,
                msg=orderInfo.msgstr,
            )

    @catch_exceptions
    def OnTradeReport(self, trade: tdapi.TradeInfo):
        """ 这个功能没有

        :param trade:
        :return:
        """
        print(f"trade: {trade.order_id}")

    @catch_exceptions
    def OnQueryPosition(self, position: tdapi.PositionInfo, req_id: str, isLast: bool):
        # TODO
        self.getLogger().info(f"symbol: {position.symbol}， "
                              f"volume： {position.volume}， "
                              f"avail_volume: {position.avail_volume}")

        if position.symbol == "" and isLast:
            self.updateAccountHoldings(dict())
            self.__holdings.clear()
            return

        holding_doc = self.__holdings.get(pRspQryPosition.securityOptionID, dict())

        #TODO: 区分多仓和空仓

        if isLast:
            # account_holding = dict()
            # for s_id, doc in self.__holdings.items():
            #     ticker = optionCode2Ticker(s_id)
            #     holding = AccountHolding(
            #         long_available=doc.get("long_available", 0),
            #         long_avg_cost=doc.get("long_avg_cost", 0),
            #         long_holding=doc.get("long_holding", 0),
            #         long_holding_td=doc.get("long_holding", 0) - doc.get("yd_long_holding", 0),
            #         long_holding_yd=doc.get("yd_long_holding", 0),
            #         long_margin=doc.get("long_margin", 0),
            #         long_profit=doc.get("long_profit", 0),
            #         short_profit=doc.get("short_profit", 0),
            #         short_available=doc.get("short_available", 0),
            #         short_avg_cost=doc.get("short_avg_cost", 0),
            #         short_holding=doc.get("short_holding", 0),
            #         short_opened_today=doc.get("short_holding_td", 0),
            #         short_holding_td=doc.get("short_holding", 0) - doc.get("yd_short_holding", 0),
            #         short_holding_yd=doc.get("yd_short_holding", 0),
            #         short_margin=doc.get("short_margin", 0),
            #     )
            #     account_holding[ticker] = holding

            # self.updateAccountHoldings(account_holding)
            self.__holdings.clear()

    @catch_exceptions
    def OnQueryAsset(self, asset: tdapi.AssetInfo, req_id: str, is_last: bool):
        self.getLogger().info(f"avail_amount: {asset.avail_amount}， total_amount： {asset.total_amount}， ")
        balance = AccountBalance(
            balance=asset.total_amount,
            cash_balance=asset.total_amount,
            cash_available=asset.avail_amount,
            margin=0,
            unrealized_pnl=0,
            realized_pnl=0,
        )

        self.updateAccountBalance({"CNY": balance})
        self.updateAccountHoldings(dict())

    @catch_exceptions
    def OnQueryOrder(self, order: tdapi.OrderInfo, req_id: str, is_last: bool):
        """ 煊鼎的交易都走交易服务，不涉及到查询人工报单的问题

        :param order:
        :param req_id:
        :param is_last:
        :return:
        """
        if order.status < 4 and not self.__start_canceled:
            self.api.CancelOrder(order.order_id)

        if is_last:
            self.__start_canceled = True

    # server functions start
    async def sendLimitOrder(self, order: LimitOrder):
        req = tdapi.LimitOrderRequest()
        req.symbol = order.getTicker().split('.')[0]
        req.price = order.getLimitPrice()
        req.qty = order.getQuantity()
        req.side = 1 if order.isBuy() else 2
        req.order_id = generate_uid()

        order.setCustId(req.order_id)

        self.getLogger().info(f"send order: {req.order_id}, ticker: {req.symbol}, ")

        self.api.PlaceOrder(req)

    async def cancelOrder(self, order: LimitOrder):
        if order.getExchangeId() is not None:
            self.api.CancelOrder(order.getExchangeId())

    async def sendOrdersInBatch(self, batch_id, order_list: typing.List[LimitOrder]):
        for order in order_list:
            await self.sendLimitOrder(order)

    async def cancelOrdersInBatch(self, order_list: typing.List[LimitOrder]):
        for order in order_list:
            await self.cancelOrder(order)

    async def qryAccountHolding(self):
        self.api.QueryPosition(self.getReq())

    async def qryActiveOrder(self):
        self.api.QueryOrder(self.getReq())

    async def sendEtfConvert(self, etf_convert: EtfConvertRequest):
        raise NotImplementedError

    def login(self, *args, **kwargs):
        req = tdapi.LoginRequest()

        # 测试
        # req.user_type = 1
        # req.user_id = "fk0001"
        # req.user_passw = "21218CCA77804D2BA1922C33E0151105"
        #
        # connReq = tdapi.ConnRequest()
        # connReq.ser_type = "tcp"
        # connReq.ser_url = "106.15.74.182"
        # connReq.ser_port = 10001
        # connReq.ser_pubkem = "./config/pubkey.pem"
        # connReq.ser_aes = "ABCDEFGHABCDEFGH"

        # 实盘
        req.user_type = 6
        req.user_id = "TGGZX"
        req.user_passw = "948e8600da3bb7a443b25d07ab55bfe6"

        connReq = tdapi.ConnRequest()
        connReq.ser_type = "tcp"
        connReq.ser_url = "106.14.70.60"
        connReq.ser_port = 10001
        connReq.ser_pubkem = "./config/pubkey.pem"
        connReq.ser_aes = "ABCDEFGHABCDEFGH"

        resp = self.api.LoginTrade(connReq=connReq, request=req)

        if resp.is_success:
            self.addCoroutineTask(self.qryActiveOrder())
            self.addCoroutineTask(self.qryAccountHolding())
            self.addCoroutineTask(self.qryAccountBalance())

            return True

        else:
            return False

    async def qryAccountBalance(self):
        self.api.QueryAsset(self.getReq())


if __name__ == '__main__':
    s = TradeServer(hostname='tradeserver-xd-test')
    s.getLogger().setLevel('DEBUG')
    s.run_forever()

    import time
    time.sleep(999999)