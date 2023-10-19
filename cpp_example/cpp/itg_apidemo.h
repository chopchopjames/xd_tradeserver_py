#include "TradeApi.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace ITG;

class ItgApiDemo: public TradeSpi 
{
public:
    ItgApiDemo() {}
public:
    virtual void OnOrderReport(const ITG::OrderInfo* confirm) ;
    virtual void OnTradeReport(const TradeInfo * fill) ;
    virtual void OnQueryOrder(const OrderInfo *order_info, std::string request_id, bool is_last) ;
    virtual void OnQueryTrade(const TradeInfo *trade_info,  std::string request_id, bool is_last) ;
    virtual void OnQueryPosition(const PositionInfo *pos_info,  std::string request_id, bool is_last) ;
    virtual void OnQueryAsset(const AssetInfo *asset_info, std::string request_id, bool is_last) ;
	virtual void OnQuotData(const QuotationData *quot_data) ;
	virtual void OnQuotDeal(const QuotationDeal *quot_deal) ;
	virtual void OnQuotOrder(const QuotationOrder *quot_order) ;
	virtual void OnQuotStatic(const StaticData *static_data, std::string request_id, bool is_last) ;
public:
    bool login();
    int place_order();
    int cancel_order();
    int query_orders();
    int query_trades();
    int query_position();
    int query_asset();
	int SubscribeTicker();
	int query_static();
private:
    TradeApi*   m_api;
};

