#ifndef _TRADE_INCLUDE_H
#define _TRADE_INCLUDE_H

#include<string>
#include <cstdint>

namespace ITG
{	
	enum
	{
		ITG_SUCCESS = 0,                       /**< 0.  成功 */
		ITG_ERR_NETWORK = -4,				   /**<-4 网络异常错误*/
		ITG_ERR_UNKNOWN = -99                  /**< -99. 未知错误 */

	};

	/////////////////////////////////////////////////////////////////////
    ///订单状态
    /////////////////////////////////////////////////////////////////////
    enum OrderStatus 
    {
        PENDING         =   0, //未报
        ACK             =   1, //待报
        REJECTED        =   2, //拒单
        CONFIRMED       =   3, //已报
        CANCELLED       =   4, //已撤
        PARTIAL_FILLED  =   5, //部成
        ALL_FILLED      =   6, //全成
        CANCEL_PENDING  =   7, //已报待撤
        FILLCNL_PENDING =   8, //部成待撤
        FILLCNL_END     =   9, //部成已撤
        ERROR_STU       =   10 //委托失败
    };

	/////////////////////////////////////////////////////////////////////
    ///买卖方式
    /////////////////////////////////////////////////////////////////////
    enum TradeMode 
    {
        NORMAL_BUY      =   1, //普通买入
        NORMAL_SELL     =   2, //普通卖出
    };

	/////////////////////////////////////////////////////////////////////
    ///市场类型
    /////////////////////////////////////////////////////////////////////
    enum MarketType 
    {
        SSE             =   1, //上海市场
        SZE             =   2, //深圳市场
        OTH             =   3  //其他市场
    };

	/////////////////////////////////////////////////////////////////////
    ///连接请求
    /////////////////////////////////////////////////////////////////////
	typedef struct ConnRequest
	{
		std::string ser_type;//连接类型:tcp http websocket udp等
		std::string ser_url; //连接地址
		int ser_port;//连接端口
		std::string ser_pubkem;//公钥地址
		std::string ser_aes;//aes密钥
		
		ConnRequest()
		{
			ser_type = "";
			ser_url= "";
			ser_port = 0;
			ser_pubkem = "";
			ser_aes = "";
		}
	}ConnRequest;

	/////////////////////////////////////////////////////////////////////
    ///登录请求
    /////////////////////////////////////////////////////////////////////
	typedef struct LoginRequest
	{
		std::string user_id;//用户编号
		int user_type;//用户类型 1表示交易员 6表示投顾
		std::string user_passw; //用户密码
		
		LoginRequest()
		{
			user_id = "";
			user_type = 0;
			user_passw = "";
		}
	}LoginRequest;

	/////////////////////////////////////////////////////////////////////
    ///消息应答
    /////////////////////////////////////////////////////////////////////
	typedef struct MsgResponse
	{
		bool is_success; //是否成功，true表示成功，false表示失败
		std::string id_msg; //应答信息
		
		MsgResponse()
		{
			is_success = false;
			id_msg = "";
		}
	}MsgResponse;

	/////////////////////////////////////////////////////////////////////
    ///限价委托请求
    /////////////////////////////////////////////////////////////////////
	typedef struct LimitOrderRequest
	{
		std::string order_id;//委托ID
		std::string symbol; //股票代码
		double price; //委托价格
		double qty; //委托数量
		int side;//买卖方式
		int exchange;//股票市场 
		
		LimitOrderRequest()
		{
			order_id = "";
			symbol = "";
			price = 0.0;
			qty = 0;
		}
	}LimitOrderRequest;

    /////////////////////////////////////////////////////////////////////
    ///成交消息
    /////////////////////////////////////////////////////////////////////
    typedef struct TradeInfo
	{
		std::string order_id;//委托ID
		std::string timestamp; //成交时间
		double price; //成交均价
		double qty; //成交数量
		std::string deal_id;//成交编号
		std::string symbol;//股票代码
		int side;//买卖方向
		
		TradeInfo()
		{
			order_id = "";
			timestamp = "";
			price = 0.0;
			qty = 0;
			deal_id = "";
			symbol = "";
			side = -1;
		}
	}TradeInfo;    

    /////////////////////////////////////////////////////////////////////
    ///订单状态
    /////////////////////////////////////////////////////////////////////
    typedef struct OrderInfo
	{
		std::string order_id;//委托ID
		std::string symbol; //股票代码
		double price; //委托价格
		double qty; //委托数量
		int side;//买卖方式
		int exchange;//股票市场
		double filled_price; //成交均价
		double filled_qty; //委托数量
		std::string status;//委托状态
		double cxled_qty;//撤单数量
		std::string ordertime;
		std::string msgstr;
		std::string broker_id;//券商实际id
		
		OrderInfo()
		{
			order_id = "";
			symbol = "";
			price = 0.0;
			qty = 0.0;
			filled_price = 0.0;
			filled_qty = 0;
			cxled_qty = 0;
			ordertime = "";
			msgstr = "";
			broker_id = "";
		}
	}OrderInfo;

	/////////////////////////////////////////////////////////////////////
    ///仓位明细
    /////////////////////////////////////////////////////////////////////
    typedef struct PositionInfo
	{
		std::string user_id;//用户编号
		std::string symbol; //股票代码
		double ref_cost; //参考成本
		double volume; //总持仓量
		double avail_volume; //可用持仓量
		
		PositionInfo()
		{
			user_id = "";
			symbol = "";
			ref_cost = 0.0;
			volume = 0.0;
			avail_volume = 0.0;
		}
	}PositionInfo;

	/////////////////////////////////////////////////////////////////////
    ///账号资金
    /////////////////////////////////////////////////////////////////////
    typedef struct AssetInfo
	{
		std::string user_id;//用户编号
		double avail_amount; //可用资金
		double total_amount; //总资金
		
		AssetInfo()
		{
			user_id = "";
			avail_amount = 0.0;
			total_amount = 0.0;
		}
	}AssetInfo;

	/////////////////////////////////////////////////////////////////////
    ///盘口数据
    /////////////////////////////////////////////////////////////////////
	typedef struct QuotationData
	{
		std::string orderTime;//订单时间
		std::string stockCode;//证券代码
		double preClosePrice;//昨收价
		double openPrice;//开盘价
		double highPrice;//最高价
		double lowPrice;//最低价
		double lastPrice;//现价
		double closePrice;//收盘价
		long totalVolume;//总交易量
		double totalAmount;//总交易金额
		
		double sellPrice01;//卖一价
		int sellVolume01;//卖一量
		double sellPrice02;//卖二价
		int sellVolume02;//卖二量
		double sellPrice03;//卖三价
		int sellVolume03;//卖三量
		double sellPrice04;//卖四价
		int sellVolume04;//卖四量
		double sellPrice05;//卖五价
		int sellVolume05;//卖五量
		double sellPrice06;//卖六价
		int sellVolume06;//卖六量
		double sellPrice07;//卖七价
		int sellVolume07;//卖七量
		double sellPrice08;//卖八价
		int sellVolume08;//卖八量
		double sellPrice09;//卖九价
		int sellVolume09;//卖九量
		double sellPrice10;//卖十价
		int sellVolume10;//卖十量
		
		double buyPrice01;//买一价
		int buyVolume01;//买一量
		double buyPrice02;//买二价
		int buyVolume02;//买二量
		double buyPrice03;//买三价
		int buyVolume03;//买三量
		double buyPrice04;//买四价
		int buyVolume04;//买四量
		double buyPrice05;//买五价
		int buyVolume05;//买五量
		double buyPrice06;//买六价
		int buyVolume06;//买六量
		double buyPrice07;//买七价
		int buyVolume07;//买七量
		double buyPrice08;//买八价
		int buyVolume08;//买八量
		double buyPrice09;//买九价
		int buyVolume09;//买九量
		double buyPrice10;//买十价
		int buyVolume10;//买十量
		
		QuotationData()
		{
			orderTime = "";//订单时间
			stockCode = "";//证券代码
			preClosePrice = 0.0;//昨收价
			openPrice = 0.0;//开盘价
			highPrice = 0.0;//最高价
			lowPrice = 0.0;//最低价
			lastPrice = 0.0;//现价
			closePrice = 0.0;//收盘价
			totalVolume = 0;//总成交数量
			totalAmount = 0.0;//总成交金额
			
			sellPrice01 = 0.0;//卖一价
			sellVolume01 = 0;//卖一量
			sellPrice02 = 0.0;//卖二价
			sellVolume02 = 0;//卖二量
			sellPrice03 = 0.0;//卖三价
			sellVolume03 = 0;//卖三量
			sellPrice04 = 0.0;//卖四价
			sellVolume04 = 0;//卖四量
			sellPrice05 = 0.0;//卖五价
			sellVolume05 = 0;//卖五量
			sellPrice06 = 0.0;//卖六价
			sellVolume06 = 0;//卖六量
			sellPrice07 = 0.0;//卖七价
			sellVolume07 = 0;//卖七量
			sellPrice08 = 0.0;//卖八价
			sellVolume08 = 0;//卖八量
			sellPrice09 = 0.0;//卖九价
			sellVolume09 = 0;//卖九量
			sellPrice10 = 0.0;//卖十价
			sellVolume10 = 0;//卖十量
			
			buyPrice01 = 0.0;//买一价
			buyVolume01 = 0;//买一量
			buyPrice02 = 0.0;//买二价
			buyVolume02 = 0;//买二量
			buyPrice03 = 0.0;//买三价
			buyVolume03 = 0;//买三量
			buyPrice04 = 0.0;//买四价
			buyVolume04 = 0;//买四量
			buyPrice05 = 0.0;//买五价
			buyVolume05 = 0;//买五量
			buyPrice06 = 0.0;//买六价
			buyVolume06 = 0;//买六量
			buyPrice07 = 0.0;//买七价
			buyVolume07 = 0;//买七量
			buyPrice08 = 0.0;//买八价
			buyVolume08 = 0;//买八量
			buyPrice09 = 0.0;//买九价
			buyVolume09 = 0;//买九量
			buyPrice10 = 0.0;//买十价
			buyVolume10 = 0;//买十量
		}
	}QuotationData;

	/////////////////////////////////////////////////////////////////////
    ///逐笔成交
    /////////////////////////////////////////////////////////////////////
	typedef struct QuotationDeal
	{
		std::string stockCode;//证券代码
		int direction;//买卖方向1表示买入，2表示卖出
		std::string recID;//昨收价
		std::string buyRecID;//买入id
		std::string sellRecID;//卖出id
		double dealPrice;//成交价
		int dealCount;//成交数量
		std::string tradeType;//成交类型
		std::string dealTime;//成交时间
		std::string exchangeCode;//市场代码SZ/SH
		
		QuotationDeal()
		{
			stockCode = "";
			direction = 0;
			recID = "";
			buyRecID = "";
			sellRecID = "";
			dealPrice = 0.0;
			dealCount = 0;
			tradeType = "";
			dealTime = "";
			exchangeCode = "";
		}
	}QuotationDeal;

	/////////////////////////////////////////////////////////////////////
    ///逐笔委托
    /////////////////////////////////////////////////////////////////////
	typedef struct QuotationOrder
	{
		std::string stockCode;//证券代码
		std::string recID;//消息记录号:从 1 开始计数，同一频道连续
		double orderPrice;//委托价格
		int orderVolume;//委托数量
		std::string orderCode;//买卖方向：1=买 2=卖 G=借入 F=出借
		std::string orderType;//订单类别：1=市价 2=限价 U=本方最优
		std::string time;//时间
		
		QuotationOrder()
		{
			stockCode = "";
			recID = "";
			orderPrice = 0.0;
			orderVolume = 0;
			orderCode = "";
			orderType = "";
			time = "";
		}
	}QuotationOrder;

	/////////////////////////////////////////////////////////////////////
    ///静态码表
    /////////////////////////////////////////////////////////////////////
	typedef struct StaticData
	{
		std::string stockCode;//股票代码
		std::string stockName;//证券名称
		double preClosePrice;//昨收价
		int listingDate;//静态数量里面的上市日期
		std::string staticDate;//静态日期时间
		std::string pinyin;//拼音缩写
		double priceUpLimit;//涨停价
		double priceDownLimit;//跌停价
		StaticData()
		{
			stockCode = "";
			stockName = "";
			preClosePrice = 0.0;
			listingDate = 0;
			staticDate = "";
			pinyin = "";
			priceUpLimit = 0.0;
			priceDownLimit = 0.0;
		}
	}StaticData;
}//namespace
#endif // _TRADE_INCLUDE_H

