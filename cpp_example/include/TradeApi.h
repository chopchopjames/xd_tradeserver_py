#ifndef _TRADE_API_H
#define _TRADE_API_H

#include "TradeInclude.h"
#include <string>

class TradeSpi
{
public:
	/// 委托回报
    virtual void OnOrderReport(const ITG::OrderInfo* confirm) = 0;
    /// 成交回报
    virtual void OnTradeReport(const ITG::TradeInfo* fill) = 0;
    /// 持仓查询
    virtual void OnQueryPosition(const ITG::PositionInfo *pos_info,  std::string request_id, bool is_last) = 0;
    /// 资金查询
    virtual void OnQueryAsset(const ITG::AssetInfo *asset_info, std::string request_id, bool is_last) = 0;
    /// 订单查询
    virtual void OnQueryOrder(const ITG::OrderInfo *order_info, std::string request_id, bool is_last) = 0;
    /// 成交查询
    virtual void OnQueryTrade(const ITG::TradeInfo *trade_info, std::string request_id, bool is_last) = 0;
	/// 行情推送
    virtual void OnQuotData(const ITG::QuotationData *quot_data) = 0;
	/// 成交推送
    virtual void OnQuotDeal(const ITG::QuotationDeal *quot_deal) = 0;
	/// 委托推送
    virtual void OnQuotOrder(const ITG::QuotationOrder *quot_order) = 0;
	/// 静态推送
    virtual void OnQuotStatic(const ITG::StaticData *static_data, std::string request_id, bool is_last) = 0;
};

class TradeApi
{
public:
	TradeApi();
	~TradeApi();
    /// 注册回调接口
    /// spi 派生自回调接口类的实例，请在登录之前设定
    virtual void RegisterSpi(TradeSpi *spi);
    /// 登录，同步返回登录结果
    virtual ITG::MsgResponse LoginTrade(ITG::ConnRequest connReq, const ITG::LoginRequest request);
	/// 登录，同步返回登录结果
    virtual ITG::MsgResponse LoginQuot(ITG::ConnRequest connReq, const ITG::LoginRequest request);
    /// 报单，返回0表示发送成功
    virtual int PlaceOrder(const ITG::LimitOrderRequest *order);
    /// 撤单，返回0表示发送成功
    virtual int CancelOrder(std::string order_id);
    //查询可用资金信息，返回0表示请求成功
    virtual int QueryAsset(std::string request_id);
    //查询持仓， 返回0表示请求成功
    virtual int QueryPosition(std::string request_id);
    /// 查询当天所有委托，返回0表示请求成功
    virtual int QueryOrder(std::string request_id);
    /// 查询当天所有成交，返回0表示请求成功
    virtual int QueryTrade(std::string request_id);
	/// 查询当日的静态数据，第二天的静态数据要上午九点零五分才会更新
	virtual int QueryStaticDatas();
	/// 订阅行情数据，类型1表示盘口，2表示盘口+逐笔成交，3表示盘口+逐笔成交+逐笔委托
	virtual int SubscribeTicker(std::string symbol, int type);
public:
	TradeSpi *m_spi;
};


#endif // _TRADE_API_H

