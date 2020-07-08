#include "UProtocol.h"

void TestUptrade()
{
    UPUptrade up;
    up.set_token("123");
    up.set_paypassword("456");
    up.set_marketid(1);
    up.set_price(2);
    up.set_num(3);
    up.set_type(4);
    up.set_main_coin(5);

    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPUptrade>(&up,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<<buffer<<endl;

    UPUptrade up2;
    JsonUnpack<UPUptrade>(&up2,buffer,iLen);
    cout<<"token:"<<up2.token()<<endl;
    cout<<"paypassword:"<<up2.paypassword()<<endl;
    cout<<"marketid:"<<up2.marketid()<<endl;
    cout<<"price:"<<up2.price()<<endl;
    cout<<"num:"<<up2.num()<<endl;
    cout<<"type:"<<up2.type()<<endl;
    cout<<"main_coin:"<<up2.main_coin()<<endl;

}
void TestUptradeBatch()
{
    UPUptradebatch upb;
    UPUptradebatch::Trades* pTrades = NULL;
    upb.set_paypassword("123456");
    for(int i=0; i < 2; ++i)
    {   
        pTrades = upb.add_trades();
        pTrades->set_marketid(222);
        pTrades->set_price(333);
        pTrades->set_num(666);
        pTrades->set_type(888);
    }
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPUptradebatch>(&upb,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<<buffer<<endl;

    UPUptradebatch upb2;
    JsonUnpack<UPUptradebatch>(&upb2,buffer,iLen);
    cout<<"paypassword:"<<upb2.paypassword()<<endl;;
    for(int i = 0; i < upb2.trades_size(); ++i)
    {
        const UPUptradebatch::Trades& item = upb2.trades(i);
        cout<<"marketid:"<<item.marketid()<<endl;
        cout<<"price:"<<item.price()<<endl;
        cout<<"num:"<<item.num()<<endl;
        cout<<"type:"<<item.type()<<endl;
    }
}
void TestAllmarketinfo()
{
    UPAllmarketinfo marketinfo;
    marketinfo.set_type("hello world");
    UPAllmarketinfo::Data* pData = NULL;
    for(uint32_t i = 0; i < 2; ++i)
    {
        pData = marketinfo.add_data();
        pData->set_m0("12");
        pData->set_m1("13");
        pData->set_m2("14");
        pData->set_m3("15");
        pData->set_m4("16");
        pData->set_m5("17");
        pData->set_m6("18");
        pData->set_m7("19");
        pData->set_m8("20");
        pData->set_m9("21");
        pData->set_m10("115");
        pData->set_m11("116");
        pData->set_m12("117");
        pData->set_m13("118");
    }
    marketinfo.set_marketid(123456);
    UPAllmarketinfo::Exchange* pExchange = NULL;
    pExchange = marketinfo.mutable_exchange();
    pExchange->set_btc("BTC");
    pExchange->set_eth("ETH");
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPAllmarketinfo>(&marketinfo,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<< UPAllmarketinfo::CMD <<endl;
    cout<<buffer<<endl;

    UPAllmarketinfo marketinfo2;
    JsonUnpack<UPAllmarketinfo>(&marketinfo2,buffer,iLen);
    cout<<"type:"<<marketinfo2.type()<<endl;
    for(int i = 0; i < marketinfo2.data_size(); ++i)
    {
        const UPAllmarketinfo::Data& item = marketinfo2.data(i);
        cout<<"m0"<<item.m0()<<endl;
        cout<<"m1"<<item.m1()<<endl;
        cout<<"m2"<<item.m2()<<endl;
        cout<<"m3"<<item.m3()<<endl;
        cout<<"m4"<<item.m4()<<endl;
        cout<<"m5"<<item.m5()<<endl;
        cout<<"m6"<<item.m6()<<endl;
        cout<<"m7"<<item.m7()<<endl;
        cout<<"m8"<<item.m8()<<endl;
        cout<<"m9"<<item.m9()<<endl;
        cout<<"m10"<<item.m10()<<endl;
        cout<<"m11"<<item.m11()<<endl;
        cout<<"m12"<<item.m12()<<endl;
        cout<<"m13"<<item.m13()<<endl;	
    }
    cout<<"marketid:"<<marketinfo2.marketid()<<endl;
    UPAllmarketinfo::Exchange exc = marketinfo2.exchange();
    cout<<"btc:"<<exc.btc()<<endl;
    cout<<"eth:"<<exc.eth()<<endl;

}
void TestDepthdata()
{
    UPDepthdata depth;
    depth.set_type("world");
    UPDepthdata::Data* pData = depth.mutable_data();
    UPDepthdata::Data::Depth* pDepth = pData->mutable_depth();
    UPDepthdata::Data::Depth::buyAction* pBuy = NULL;
    UPDepthdata::Data::Depth::sellAction* pSell = NULL;
    for(uint32_t i = 0; i < 2; ++i)
    {
        pBuy = pDepth->add_buy();   
        pBuy->set_b0("11");
        pBuy->set_b1("22");
    }
    for(uint32_t j = 0; j < 2; ++j)
    {
        pSell = pDepth->add_sell();
        pSell->set_s0("66");
        pSell->set_s1("88");
    }
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPDepthdata>(&depth,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<< UPDepthdata::CMD <<endl;
    cout<<buffer<<endl;

    UPDepthdata depth2;
    JsonUnpack<UPDepthdata>(&depth2,buffer,iLen);
    cout<<"type:"<<depth2.type()<<endl;
    UPDepthdata::Data tmpData = depth2.data();
    UPDepthdata::Data::Depth tmpDepth = tmpData.depth();
    for(int i = 0; i < tmpDepth.buy_size(); ++i)
    {
        const UPDepthdata::Data::Depth::buyAction& tmpBuy = tmpDepth.buy(i);
        cout<<"buy0:"<<tmpBuy.b0()<<endl;
        cout<<"buy1:"<<tmpBuy.b1()<<endl;
    }
    for(int j = 0; j < tmpDepth.sell_size(); ++j)
    {
        const UPDepthdata::Data::Depth::sellAction& tmpSell = tmpDepth.sell(j);
        cout<<"sell0:"<<tmpSell.s0()<<endl;
        cout<<"sell1:"<<tmpSell.s1()<<endl;
    }
}

void TestHistoricalTransactionData()
{
    UPHistoricalTransactionData hist;
    hist.set_type("hello world");
    UPHistoricalTransactionData::Data* pData = hist.mutable_data();
    UPHistoricalTransactionData::Data::TradeLog *pTradeLog = NULL;
    for(uint32_t i = 0; i < 2; ++i)
    {
        pTradeLog = pData->add_tradelog();
        pTradeLog->set_tradeid(999999);
        pTradeLog->set_addtime("aaa");
        pTradeLog->set_timestamp("54645");
        pTradeLog->set_type(123456);
        pTradeLog->set_price("122");
        pTradeLog->set_mum("bbb");
        pTradeLog->set_num("ccc");      
    }
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPHistoricalTransactionData>(&hist,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<<buffer<<endl;
    UPHistoricalTransactionData hist2;
    JsonUnpack<UPHistoricalTransactionData>(&hist2,buffer,iLen);
    cout<<"type:"<<hist2.type()<<endl;
    UPHistoricalTransactionData::Data tmpData = hist2.data();
    for(uint32_t i = 0; i < tmpData.tradelog_size(); ++i)
    {
        const UPHistoricalTransactionData::Data::TradeLog& tmpTradeLog = tmpData.tradelog(i);
        cout<<"tradeid:"<<tmpTradeLog.tradeid()<<endl;
        cout<<"addtime:"<<tmpTradeLog.addtime()<<endl;
        cout<<"timestamp:"<<tmpTradeLog.timestamp()<<endl;
        cout<<"type:"<<tmpTradeLog.type()<<endl;
        cout<<"mum:"<<tmpTradeLog.mum()<<endl;
        cout<<"num:"<<tmpTradeLog.num()<<endl;         
    }
}

void TestKlinedata()
{
    UPKlinedata kline;
    kline.set_type("hello world");
    UPKlinedata::Data* pData = NULL;
    for(uint32_t i = 0; i < 4; ++i)
    {
        UPKlinedata::Data* pDataTest = new UPKlinedata::Data;
        pDataTest->set_k0("000");
        pDataTest->set_k1("111");
        pDataTest->set_k2("222");
        pDataTest->set_k3("333");
        pDataTest->set_k4("444");
        pDataTest->set_k5("555");  

        pData = kline.add_data();
        pData->Swap(pDataTest);      
    }
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPKlinedata>(&kline,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<<buffer<<endl;

    UPKlinedata kline2;
    JsonUnpack<UPKlinedata>(&kline2,buffer,iLen);
    cout<<"type:"<<kline2.type()<<endl;
    for(uint32_t i = 0; i < kline2.data_size(); ++i)
    {
        const UPKlinedata::Data& tmpData = kline2.data(i);
        cout<<"k0:"<<tmpData.k0()<<endl;
        cout<<"k1:"<<tmpData.k1()<<endl;
        cout<<"k2:"<<tmpData.k2()<<endl;
        cout<<"k3:"<<tmpData.k3()<<endl;
        cout<<"k4:"<<tmpData.k4()<<endl;
        cout<<"k5:"<<tmpData.k5()<<endl;         
    }
}

void TestMatchedData()
{
    UPMatchedData match;
    match.set_addtime("777");
    match.set_id(6666);
    match.set_price("999");
    UPMatchedData::BuyUser* bUser = match.mutable_buyuser();
    bUser->set_userid("111");
    bUser->set_num("222");
    bUser->set_deal("333");
    bUser->set_cny(555);
    bUser->set_cnyd(666);

    UPMatchedData::SellUser* sUser = match.mutable_selluser();
    sUser->set_userid("11");
    sUser->set_num("22");
    sUser->set_deal("33");
    sUser->set_xnb(88);
    sUser->set_xnbd(99);
    const int BUFFSIZE=1024;
    char buffer[BUFFSIZE];
    int iLen=JsonPack<UPMatchedData>(&match,buffer,BUFFSIZE);
    buffer[iLen]='\0';
    cout<<buffer<<endl;

    UPMatchedData match1;
    JsonUnpack<UPMatchedData>(&match1,buffer,iLen);
    cout<<"addtime:"<<match1.addtime()<<endl;
    cout<<"id:"<<match1.id()<<endl;
    cout<<"price:"<<match1.price()<<endl;
    UPMatchedData::BuyUser tmpBuy = match1.buyuser();
    cout<<"userid:"<<tmpBuy.userid()<<endl;
    cout<<"num:"<<tmpBuy.num()<<endl;
    cout<<"deal:"<<tmpBuy.deal()<<endl;
    cout<<"cny:"<<tmpBuy.cny()<<endl;
    cout<<"cnyd:"<<tmpBuy.cnyd()<<endl;
    UPMatchedData::SellUser tmpSell = match1.selluser();
    cout<<"userid:"<<tmpSell.userid()<<endl;
    cout<<"num:"<<tmpSell.num()<<endl;
    cout<<"deal:"<<tmpSell.deal()<<endl;
    cout<<"xnb:"<<tmpSell.xnb()<<endl;
    cout<<"xnbd:"<<tmpSell.xnbd()<<endl;

    int cmd = UPMatchedData::CMD;
    cout<< "CMD : "<< cmd<<endl;
}

void AesAPITest()
{
    unsigned char *date = new unsigned char[AES_BLOCK_SIZE];
    unsigned char *encrypt = new unsigned char[AES_BLOCK_SIZE];
    unsigned char *plain = new unsigned char[AES_BLOCK_SIZE];
    //memset((void*)date, 'p', AES_BLOCK_SIZE);
    char tmp[17] = "1585800069:11109";
    memcpy((void*)date, tmp, AES_BLOCK_SIZE);
    memset((void*)encrypt, 0, AES_BLOCK_SIZE);
    memset((void*)plain, 0, AES_BLOCK_SIZE);
    AesEncrypt(encrypt, AES_BLOCK_SIZE, date,  AES_BLOCK_SIZE);
    AesDecrypt(plain, AES_BLOCK_SIZE, encrypt, AES_BLOCK_SIZE);
    printf("encrypt:");
    for(int i = 0; i < AES_BLOCK_SIZE; i++){
        printf("%.2x", encrypt[i]);
        if((i+1) % 32 == 0){
            printf("\n");    
        }
    }
    printf("\ndecrypt:%s",plain);
    printf("\n"); 
}
void AesDemo()
{
    char userkey[AES_BLOCK_SIZE];
    unsigned char *date = (unsigned char *)malloc(AES_BLOCK_SIZE*3);
    unsigned char *encrypt = (unsigned char *)malloc(AES_BLOCK_SIZE*4 + 4);
    unsigned char *plain = (unsigned char *)malloc(AES_BLOCK_SIZE*3);
    AES_KEY key;

    memset((void*)userkey, 'k', AES_BLOCK_SIZE);
    memset((void*)date, 'p', AES_BLOCK_SIZE*3);
    memset((void*)encrypt, 0, AES_BLOCK_SIZE*4+4);
    memset((void*)plain, 0, AES_BLOCK_SIZE*3);

    /*设置加密key及密钥长度*/
    AES_set_encrypt_key((const unsigned char *)userkey, AES_BLOCK_SIZE*8, &key);

    unsigned char* in_mem_need = new unsigned char[AES_BLOCK_SIZE*4];
    memset((void*)in_mem_need,0,AES_BLOCK_SIZE*4);
    strncpy((char*)in_mem_need,(const char*)date,AES_BLOCK_SIZE*3);

    int len = 0;
    /*循环加密，每次只能加密AES_BLOCK_SIZE长度的数据*/
    while(len < AES_BLOCK_SIZE*4) {
        AES_encrypt(in_mem_need+len, encrypt+len, &key);    
        len += AES_BLOCK_SIZE;
    }
    /*设置解密key及密钥长度*/    
    AES_set_decrypt_key((const unsigned char *)userkey, AES_BLOCK_SIZE*8, &key);

    len = 0;
    /*循环解密*/
    while(len < AES_BLOCK_SIZE*4) {
        AES_decrypt(encrypt+len, plain+len, &key);    
        len += AES_BLOCK_SIZE;
    }
    /*解密后与原数据是否一致*/
    if(!memcmp(plain, date, AES_BLOCK_SIZE*3)){
        printf("test success\n");    
    }else{
        printf("test failed\n");    
    }

    printf("encrypt: ");
    int i = 0;
    for(i = 0; i < AES_BLOCK_SIZE*4 + 4; i++){
        printf("%.2x", encrypt[i]);
        if((i+1) % 32 == 0){
            printf("\n");    
        }
    }
    printf("\n");    
    delete in_mem_need;
    in_mem_need = NULL;
}

void MD5_API_Test()
{
    unsigned char src[] = "123456";
    unsigned char *dst = new unsigned char[32];
    MD5(dst,src,strlen((const char*)src));
    cout<<"dst:"<<dst<<endl;
}

void GMT_API_Test()
{
    char pGMTTime[50] = {0};
    GetGMTime(pGMTTime,50);
    cout<<"Date :"<<pGMTTime<<endl;
}

int main()
{
    // TestUptrade();
    // TestUptradeBatch();
    // TestAllmarketinfo();
    // TestDepthdata();
    // TestHistoricalTransactionData();
    // TestKlinedata();
    // TestMatchedData();
    AesAPITest();
    MD5_API_Test();
    GMT_API_Test();
    return 0;
}


