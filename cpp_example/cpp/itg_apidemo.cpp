#include <iostream>
#include "itg_apidemo.h"

void ItgApiDemo::OnOrderReport(const OrderInfo* confirm) 
{
    std::cout << "OrderReport order-confirm : order_id="<<confirm->order_id << std::endl;
}

void ItgApiDemo::OnTradeReport(const TradeInfo * fill) 
{
    std::cout << "OnTradeEvent : order_id="<<fill->order_id<< std::endl;
}

void ItgApiDemo::OnQueryOrder(const OrderInfo *order_info, std::string request_id, bool is_last) 
{
    std::cout <<"OnQueryOrder: request_id="<< request_id << "|no data|is_last="<<is_last<<std::endl;
}

void ItgApiDemo::OnQueryTrade(const TradeInfo *trade_info,  std::string request_id, bool is_last) 
{
    std::cout <<"OnQueryTrade:request_id="<< request_id << "|no data|is_last="<<is_last<<std::endl;
}

void ItgApiDemo::OnQueryPosition(const PositionInfo *pos_info,  std::string request_id, bool is_last) 
{
	std::cout <<"OnQueryPosition:request_id="<< request_id << "|no data|is_last="<<is_last<<std::endl;
}

void ItgApiDemo::OnQueryAsset(const AssetInfo *asset_info, std::string request_id, bool is_last) 
{
    std::cout <<"OnQueryAsset:request_id="<< request_id << "|no data|is_last="<<is_last<<std::endl;
}

void ItgApiDemo::OnQuotData(const QuotationData *quot_data) 
{
    std::cout <<"OnQuotData:quot_data="<< quot_data->lastPrice << "|symbol="<<quot_data->stockCode<<std::endl;
}

void ItgApiDemo::OnQuotDeal(const QuotationDeal *quot_deal) 
{
    std::cout <<"OnQuotDeal:quot_deal="<< quot_deal->dealPrice << "|symbol="<<quot_deal->stockCode<<std::endl;
}

void ItgApiDemo::OnQuotOrder(const QuotationOrder *quot_order) 
{
    std::cout <<"OnQuotOrder:quot_order="<< quot_order->orderPrice << "|symbol="<<quot_order->stockCode<<std::endl;
}

void ItgApiDemo::OnQuotStatic(const StaticData *static_data, std::string request_id, bool is_last) 
{
    std::cout <<"OnQuotStatic:static_data="<< static_data->stockCode << "|no data|is_last="<<is_last<<std::endl;
}

bool  ItgApiDemo::login() 
{
    m_api = new TradeApi();
    m_api->RegisterSpi(this);

	//日内交易员
    	LoginRequest req;
	req.user_id = "fk0001";
	req.user_type = 1;//1表示交易员6表示投顾，9表示行情账户
	req.user_passw = "21218CCA77804D2BA1922C33E0151105";

	//投顾交易员
	//LoginRequest req;
	//req.user_id = "t10001";
	//req.user_type = 6;//1表示交易员6表示投顾，9表示行情账户
	//req.user_passw = "21218CCA77804D2BA1922C33E0151105";

	ConnRequest connReq;
	connReq.ser_type = "tcp";
	connReq.ser_url= "106.15.74.182";
	connReq.ser_port = 10001;
	connReq.ser_pubkem = "./pubkey.pem";
	connReq.ser_aes = "ABCDEFGHABCDEFGH";
	
    MsgResponse resp = m_api->LoginTrade(connReq, req);
    if(resp.is_success)  
	{
        std::cout <<"login success!"<<std::endl;
    }
    else
    {
        std::cout <<"login failed! reason :" << resp.id_msg<< std::endl;
        return false;
    }
/*
	//行情部分操作，不需要行情可直接省略不调用即可
	LoginRequest reqquot;
	reqquot.user_id = "t10001";
	reqquot.user_type = 9;//1表示交易员6表示投顾，9表示行情账户
	reqquot.user_passw = "21218CCA77804D2BA1922C33E0151105";

	ConnRequest connReqQuot;
	connReqQuot.ser_type = "tcp";
	connReqQuot.ser_url= "47.100.39.28";
	connReqQuot.ser_port = 10001;
	connReqQuot.ser_pubkem = "/home/itg/itg/bin/pubkey.pem";
	connReqQuot.ser_aes = "ABCDEFGHABCDEFGH";


	MsgResponse respquot = m_api->LoginQuot(connReqQuot, reqquot);
    if(resp.is_success)  
	{
        std::cout <<"quot login success!"<<std::endl;
    }
    else
    {
        std::cout <<"login failed! reason :" << resp.id_msg<< std::endl;
        return false;
    }
*/
	return true;
}

int ItgApiDemo::place_order() 
{
    LimitOrderRequest order;
    order.symbol = "605199";
    order.price = 3.9;
    order.qty = 100;
    order.side = ITG::TradeMode::NORMAL_BUY; //1表示买入2表示卖出
    order.order_id = "20230101101010";
    return m_api->PlaceOrder(&order);
}

int ItgApiDemo::cancel_order() 
{
    return m_api->CancelOrder("id");
}

int ItgApiDemo::query_orders()  
{
    return m_api->QueryOrder("query_orders");
}

int ItgApiDemo::query_trades() 
{
    return m_api->QueryTrade("query_trades");
}

int ItgApiDemo::query_position() 
{
    return m_api->QueryPosition("query_position");
}

int ItgApiDemo::query_asset() 
{
    return m_api->QueryAsset("query_asset");
}

int ItgApiDemo::SubscribeTicker() 
{
    return m_api->SubscribeTicker("000001", 3);
}

int ItgApiDemo::query_static() 
{
    return m_api->QueryStaticDatas();
}


int main(int args , char** argv) 
{
    ItgApiDemo test;
    if(!test.login())
    {
        return -1;
    }

    sleep(1);
    std::cout << "place_order: return "<< test.place_order() << std::endl; //下单测试
    sleep(1);
    std::cout << "cancel_order : return " << test.cancel_order() << std::endl; //撤单测试
    sleep(1);
    std::cout << "query_asset : return "<<  test.query_asset() <<std::endl;     //查询资金
    sleep(1);
    std::cout << "query_position : return "<<  test.query_position()<<std::endl;    //查询持仓
    sleep(1);
    std::cout << "query_orders : return "<<  test.query_orders()<<std::endl;        //查询当日委托
    sleep(1);
    std::cout << "query_trades : return "<<  test.query_trades()<<std::endl;        //查询当日成交
    sleep(1);
    /*
	std::cout << "SubscribeTicker : return "<<  test.SubscribeTicker()<<std::endl;        //订阅股票
    sleep(1);
	std::cout << "query_static : return "<<  test.query_static()<<std::endl;        //查看静态数据
    sleep(100);
    */
    std::cout << "test over!" <<std::endl;
    return 0;
}

