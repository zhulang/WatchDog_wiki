//
//  CenterMachineManager.h
//  ACBScanner
//
//  Created by Achilles on 2019/5/25.
//  Copyright © 2019 朱浪. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>

@class ACBScannerCongfig;

NS_ASSUME_NONNULL_BEGIN

@protocol ACBScannerPeripheralDelegate <NSObject>

/**
 @ 附设停止扫描后回调的回调方法
 */
- (void)peripheralDidStopScanning;

/**
 @ 调用附设启动扫描后回调此方法
 */
- (void)peripheralDidStartScanning;

/**
 @ 附设设备发送数据后的回调
 @  status, YES (send data successful)
 */
- (void)peripheralDidSendJsonString:(NSString *)jsonString status:(BOOL)status;

/**
 @ 附设设备barcode失败回调方法
 */
- (void)peripheralRecogniseDidFail:(NSString *)errorDescription;

/**
 @ 自上传模式下，上报数据后回调
 */
- (void)didUpload:(NSData *)data response:(NSURLResponse * _Nullable)response error:(NSError * _Nullable)error;

/**
 @ 连续两次扫描数据一样回调方法
 */
- (void)peripheralRecogniseSameTwice;

@end

@protocol ACBScannerCenterMachineDelegate <NSObject>

//自动上传模式，数据上传后的回调
- (void)didUpload:(NSData *)data response:(NSURLResponse * _Nullable)response error:(NSError * _Nullable)error;

//开启蓝牙之后回调
- (void)centralDidUpdateStatePoweredOn;

//成功读取值之后中心设备回调
- (void)centralDidReadValueForCharacteristic:(NSDictionary *)currentRecord;

//外设更新后回调
- (void)centralForPeripheralsUpdate:(NSMutableArray<CBPeripheral *> *)peripheralArr;

//外设连接后回调
- (void)centralDidConnectPeripheral:(CBPeripheral *)peripheral;

//链接失败的回调方法
- (void)centralDidFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error;

//取消链接后回调方法
- (void)centralDidDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error complete:(void (^)(NSMutableArray * peripheralArr))handler;

@optional
//开始扫描外设
- (void)centralDidStartScanForPeripheralsWithServices:(NSArray<CBUUID *> *)cubbids;

//发现服务后回调
- (void)centralForPeripheral:(CBPeripheral *)peripheral didDiscoverServices:(nullable NSError *)error;

//发现characteristics后回调
- (void)centralForPeripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(nullable NSError *)error;

//成功写入值之后中心设备回调
- (void)centralForPeripheral:(CBPeripheral *)peripheral didWriteValueForCharacteristic:(CBCharacteristic *)characteristic error:(nullable NSError *)error;

@end


@interface ACBScannerManager : NSObject

- (void)connectPeripheral:(CBPeripheral *)peripheral;

//获取中心设备的所有保存的扫描结果
- (NSMutableArray *)getResultData;

- (void)removeAllServices;

//开始扫描条码或二维码
- (void)beginScanningBarCode:(BOOL)resetTorch;

//停止扫描条码或二维码
- (void)stopScanningBarCode;

/**
 @ serviceName，服务名称
 @ viewController，附设代理
 @ preview，扫描区域载体
 @ previewLayerFrame，扫描区域位置
 */
- (void)initPeripheralManager:(NSString *)serviceName delegate:(id<ACBScannerPeripheralDelegate>)viewController preview:(UIView *)preview previewLayerFrame:(CGRect)previewLayerFrame;


//开始扫描附设
- (void)beginScanningPeripheral;

//停止扫描外设
- (void)stopScanningPeripheral;

/**
 @ serviceName，服务名称
 @ viewController，中心设备的代理
 */
- (void)initCenterMachineManager:(NSString *)serviceName delegate:(id<ACBScannerCenterMachineDelegate>)viewController;

@property (nonatomic,assign) CGRect previewLayerFrame;

//服务名称
@property (nonatomic,copy) NSString * serviceName;

//附设代理
@property (nonatomic,weak) id<ACBScannerPeripheralDelegate> peripheralDelegate;

//中设代理
@property (nonatomic,weak) id<ACBScannerCenterMachineDelegate> centerMachineDelegate;

+ (void)uploadData:(void (^)(BOOL success,NSString * description))handler;

+ (instancetype)manager;

//还原配置，还原配置后需要退出APP，重新打开生效
+ (void)resetConfig;

/**
 @ number ,the number of center machine connects. the value between 1 to 7 ,default is 7
 @ number ,设置中心设备连接数,最小值1,最大值7,默认值7
 */
+ (void)setCenterMaxInterfaceNumber:(NSInteger)number;
+ (NSInteger)getCenterMaxInterfaceNumber;

/**
 @ number ,cache data max number. the value between 100 to 20000 ,default is 2000
 @ number,设置最大缓存条数
 */
+ (void)setCenterMaxCacheNumber:(NSInteger)number;
+ (NSInteger)getCenterMaxCacheNumber;

/**
 @ uploadUrl ,webservice url address
 @ eg. htttps://www.webservie.com
 @ 设置缓存数据传地址
 */
+ (void)setCenterUploadUrl:(NSString *)uploadUrl;
+ (NSString *)getCenterUploadUrl;

/**
 @ dataUrl ,webservice url address.
 @ open the url to show data after uploaded data
 @ eg. htttps://www.webservice.com or htttp://www.115.239.211.112.org
 @ 设置上传数据后查看地址，没有必须要可以不用设置
 */
+ (void)setCenterDataUrl:(NSString *)dataUrl;
+ (NSString *)getCenterDataUrl;

/**
 @ autoUpload ,if set the value @(NO),the data will saved to you local device
 @ 设置中心设备是否在收到数据时自动将数据上传
 */
+ (void)setCenterAutoUpload:(BOOL)autoUpload;
+ (BOOL)getCenterAutoUpload;

/**
 @ sannerName , the name of the really scanner which is not a mobile phone
 @ 设置扫描枪名称
 */
+ (void)setScannerName:(NSString *)scannerName;
+ (NSString *)getScannerName;

/**
 @ brightness ,the value between 0.01 to 1 ,default is 0.5
 @ 设置补光亮度，值在0到1之间，默认值0.5
 */
+ (void)setPeripheralBrightness:(float)brightness;
+ (float)getPeripheralBrightness;

/**
 @ torchOn ,if set the value @(NO),the light will close,default is YES
 @ 设置是否开启补光
 */
+ (void)setPeripheralTorchOn:(BOOL)isOn;
+ (BOOL)getPeripheralTorchOn;

/**
 @ fps ,torchAuto is enable when set torchOn is YES,default is NO
 @ 设置扫描仪是否自动调节补光亮度。只有在补光开启后设置才有效
 */
+ (void)setPeripheralTorchAuto:(BOOL)isOn;
+ (BOOL)getPeripheralTorchAuto;

/**
 @ fps ,MAX value scanning times in 1 min. the value between 1 to 50 ,default is 40
 @ 设置每条扫描的最大记录条数
 */
+ (void)setPeripheralFps:(float)fps;
+ (float)getPeripheralFps;

/**
 @ focusMode ,ACCaptureFocusMode ,default is ACCaptureFocusModeLocked
 @ 设置扫描仪是否需要自动对焦
 */
+ (void)setPeripheralFocusMode:(NSInteger)focusMode;
+ (NSInteger)getPeripheralFocusMode;

/**
 @ name ,operator's name ,recommends not to set it too long
 @ 设置操作者名称
 */
+ (void)setOperatorName:(NSString *)name;
+ (NSString *)getOperatorName;

/**
 @ number ,operator's worknumber,recommends not to set it too long
 @ 设置操作者工号
 */
+ (void)setOperatorNumber:(NSString *)numStr;
+ (NSString *)getOperatorNumber;

/**
 @ isLinkScanGun,link the real scanner gun which is not a mobile phone
 @ 是否连接外部真实版扫描枪
 */
@property (nonatomic, assign) BOOL isLinkScanGun;

/**
 @ if uploadSelf == YES,upload data to service by the current device
 @ 如果设置为YES,由当前设备将扫描到数据上传
 */
@property (nonatomic, assign) BOOL uploadSelf;

/**
 @ the max value of center machine connects
 @ 中心设备连接数
 */
@property (nonatomic, assign) NSInteger maxInterfaceNumber;

/**
 @ cache data max number
 @ 最大缓存条数
 */
@property (nonatomic, assign) NSInteger maxCacheNumber;

/**
 @ webservice url address
 @ 缓存数据传地址
 */
@property (nonatomic, copy) NSString * uploadUrl;

/**
 @ webservice url address.
 @ 上传数据后查看地址
 */
@property (nonatomic, copy) NSString * dataUrl;

/**
 @ if set the value NO,the data will saved to you local device
 @ 中心设备是否在收到数据时自动将数据上传
 */
@property (nonatomic, assign) BOOL autoUpload;

/**
 @ sannerName , the name of the really scanner which is not a mobile phone
 @ 扫描枪名称
 */
@property (nonatomic, copy) NSString * sannerName;

/**
 @ brightness
 @ 设置补光亮度
 */
@property (nonatomic, assign) float brightness;

/**
 @ if set the value NO,the light will close,default is YES
 @ 是否开启补光
 */
@property (nonatomic, assign) BOOL torchOn;

/**
 @ torchAuto is enable when set torchOn is YES,default is NO
 @ 扫描仪是否自动调节补光亮度。只有在补光开启后设置才有效
 */
@property (nonatomic, assign) BOOL torchAuto;

/**
 @ MAX value scanning times in 1 min. the value between 1 to 50 ,default is 40
 @ 每条扫描的最大记录条数
 */
@property (nonatomic, assign) float fps;

/**
  @ ACCaptureFocusMode ,default is ACCaptureFocusModeLocked
  @ 扫描仪是否需要自动对焦
  */
@property (nonatomic, assign) NSInteger focusMode;

// operator's name ,recommends not to set it too long
// 操作者名称
@property (nonatomic, copy) NSString * operatorName;

// operator's worknumber,recommends not to set it too long
// 设置操作者工号
@property (nonatomic, copy) NSString * operatorNumber;

// save the data of the center device
// 中心设备上的保存的数据
@property (nonatomic,strong) NSMutableArray<NSDictionary *> * centerMachineCacheData;

// save the data of the peripheral device
// 扫描仪上的保存的数据
@property (nonatomic,strong) NSMutableArray<NSDictionary *> * peripheralMachineCacheData;

@end

NS_ASSUME_NONNULL_END
