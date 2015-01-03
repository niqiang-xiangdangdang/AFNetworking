// AFURLSessionManager.h
//
// Copyright (c) 2013-2014 AFNetworking (http://afnetworking.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>

#import "AFURLResponseSerialization.h"
#import "AFURLRequestSerialization.h"
#import "AFSecurityPolicy.h"
#import "AFNetworkReachabilityManager.h"

/**
 `AFURLSessionManager` creates and manages an `NSURLSession` object based on a specified `NSURLSessionConfiguration` object, which conforms to `<NSURLSessionTaskDelegate>`, `<NSURLSessionDataDelegate>`, `<NSURLSessionDownloadDelegate>`, and `<NSURLSessionDelegate>`.
 <ul>
 <li>`AFURLSessionManager` 基于指定的 `NSURLSessionConfiguration` 创建并管理一个 `NSURLSession` 对象</li>
 <li>遵守 `NSURLSessionTaskDelegate`、`NSURLSessionDataDelegate`、`NSURLSessionDownloadDelegate` 和 `NSURLSessionDelegate` 协议</li>
 </ul>

 ## Subclassing Notes - 子类注意事项

 This is the base class for `AFHTTPSessionManager`, which adds functionality specific to making HTTP requests. If you are looking to extend `AFURLSessionManager` specifically for HTTP, consider subclassing `AFHTTPSessionManager` instead.
 <ul>
 <li>本类是 `AFHTTPSessionManager` 类的基类</li>
 <li>`AFHTTPSessionManager` 针对 HTTP 请求添加了特定的方法</li>
 <li>如果希望针对 HTTP 协议扩展网络访问功能，建议从 `AFHTTPSessionManager` 派生子类</li>
 </ul>

 ## NSURLSession & NSURLSessionTask Delegate Methods - NSURLSession 和 NSURLSessionTask 代理方法

 `AFURLSessionManager` implements the following delegate methods:
 <br />`AFURLSessionManager` 实现了以下代理方法：

 ### `NSURLSessionDelegate`

 - `URLSession:didBecomeInvalidWithError:`
 - `URLSession:didReceiveChallenge:completionHandler:`
 - `URLSessionDidFinishEventsForBackgroundURLSession:`

 ### `NSURLSessionTaskDelegate`

 - `URLSession:willPerformHTTPRedirection:newRequest:completionHandler:`
 - `URLSession:task:didReceiveChallenge:completionHandler:`
 - `URLSession:task:didSendBodyData:totalBytesSent:totalBytesExpectedToSend:`
 - `URLSession:task:didCompleteWithError:`

 ### `NSURLSessionDataDelegate`

 - `URLSession:dataTask:didReceiveResponse:completionHandler:`
 - `URLSession:dataTask:didBecomeDownloadTask:`
 - `URLSession:dataTask:didReceiveData:`
 - `URLSession:dataTask:willCacheResponse:completionHandler:`

 ### `NSURLSessionDownloadDelegate`

 - `URLSession:downloadTask:didFinishDownloadingToURL:`
 - `URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesWritten:totalBytesExpectedToWrite:`
 - `URLSession:downloadTask:didResumeAtOffset:expectedTotalBytes:`

 If any of these methods are overridden in a subclass, they _must_ call the `super` implementation first.
 <br />如果在子类中重写了这些方法，<strong>必须</strong>先调用 `super` 的实现

 ## Network Reachability Monitoring - 网络连接状态监听

 Network reachability status and change monitoring is available through the `reachabilityManager` property. Applications may choose to monitor network reachability conditions in order to prevent or suspend any outbound requests. See `AFNetworkReachabilityManager` for more details.
 <ul>
 <li>可以通过 `reachabilityManager` 属性监听网络连接状态是否可用</li>
 <li>应用程序通过监听网络连接状态条件，可以阻止或暂停出站请求</li>
 </ul>

 ## NSCoding Caveats - NSCoding 注意事项

 - Encoded managers do not include any block properties. Be sure to set delegate callback blocks when using `-initWithCoder:` or `NSKeyedUnarchiver`.
 <ul>
 <li>被编码的管理器不包含任何 block 属性。如果使用 `-initWithCoder:` 或者 `NSKeyedUnarchiver`，需要设置代理回调的 block</li>
 </ul>
 
 ## NSCopying Caveats － NSCopying 注意事项

 - `-copy` and `-copyWithZone:` return a new manager with a new `NSURLSession` created from the configuration of the original.
 - Operation copies do not include any delegate callback blocks, as they often strongly captures a reference to `self`, which would otherwise have the unintuitive side-effect of pointing to the _original_ session manager when copied.
 <ul>
 <li>`-copy` 和 `-copyWithZone:` 方法返回一个新的管理器，并新建一个 `NSURLSession`</li>
 <li>`NSURLSession` 的配置使用原有配置</li>
 <li>操作副本不包含任何代理回调 block，这是因为在 block 中经常会出现对 `self` 的强引用，而如果拷贝 block，会在拷贝时将指针指向<strong>原有</strong>的 session 管理器</li>
 </ul>

 @warning Managers for background sessions must be owned for the duration of their use. This can be accomplished by creating an application-wide or shared singleton instance.
 <br />后台会话的管理器必须被长期持有，以保证有足够的时间完成工作，可以创建一个应用程序范围的管理器或者共享的单例来实现。
 */

#if (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED >= 70000) || (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED >= 1090)

@interface AFURLSessionManager : NSObject <NSURLSessionDelegate, NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate, NSSecureCoding, NSCopying>

/**
 The managed session.
 <br />被管理的网络会话
 */
@property (readonly, nonatomic, strong) NSURLSession *session;

/**
 The operation queue on which delegate callbacks are run.
 <br />代理回调运行的操作队列
 */
@property (readonly, nonatomic, strong) NSOperationQueue *operationQueue;

/**
 Responses sent from the server in data tasks created with `dataTaskWithRequest:success:failure:` and run using the `GET` / `POST` / et al. convenience methods are automatically validated and serialized by the response serializer. By default, this property is set to an instance of `AFJSONResponseSerializer`.
 <ul>
 <li>使用 `GET` / `POST` 等常用方法通过 `dataTaskWithRequest:success:failure:` 数据任务创建的由服务器返回的响应</li>
 <li>响应数据会被自动验证并由响应序列化器反序列化</li>
 <li>默认情况下，该属性是 `AFJSONResponseSerializer` 的实例(默认服务器返回的是 JSON 数据)</li>
 </ul>
 
 @warning `responseSerializer` must not be `nil`.
 <br />`responseSerializer` 不能为nil
 */
@property (nonatomic, strong) id <AFURLResponseSerialization> responseSerializer;

///-------------------------------
/// @name Managing Security Policy - 管理安全策略
///-------------------------------

/**
 The security policy used by created request operations to evaluate server trust for secure connections. `AFURLSessionManager` uses the `defaultPolicy` unless otherwise specified.
 <ul>
 <li>用于创建请求操作的安全策略，以建立服务器信任的安全连接</li>
 <li>除非特殊指定 `AFURLSessionManager` 使用 `defaultPolicy`</li>
 </ul>
 */
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

///--------------------------------------
/// @name Monitoring Network Reachability - 监听网络连接状态
///--------------------------------------

/**
 The network reachability manager. `AFURLSessionManager` uses the `sharedManager` by default.
 <ul>
 <li>网络连接管理器</li>
 <li>`AFURLSessionManager` 默认使用 `sharedManager`</li>
 </ul>
 */
@property (readwrite, nonatomic, strong) AFNetworkReachabilityManager *reachabilityManager;

///----------------------------
/// @name Getting Session Tasks - 获取会话任务
///----------------------------

/**
 The data, upload, and download tasks currently run by the managed session.
 <br />被管理的会话当前运行的数据、上传和下载任务
 */
@property (readonly, nonatomic, strong) NSArray *tasks;

/**
 The data tasks currently run by the managed session.
 <br />被管理的会话当前运行的数据任务
 */
@property (readonly, nonatomic, strong) NSArray *dataTasks;

/**
 The upload tasks currently run by the managed session.
 <br />被管理的会话当前运行的上传任务
 */
@property (readonly, nonatomic, strong) NSArray *uploadTasks;

/**
 The download tasks currently run by the managed session.
 <br />被管理的会话当前运行的下载任务
 */
@property (readonly, nonatomic, strong) NSArray *downloadTasks;

///-------------------------------
/// @name Managing Callback Queues - 管理回调队列
///-------------------------------

/**
 The dispatch queue for `completionBlock`. If `NULL` (default), the main queue is used.
 <ul>
 <li>`completionBlock` 的调度队列</li>
 <li>如果为 `NULL`（默认），使用主队列</li>
 </ul>
 */
@property (nonatomic, strong) dispatch_queue_t completionQueue;

/**
 The dispatch group for `completionBlock`. If `NULL` (default), a private dispatch group is used.
 <ul>
 <li>`completionBlock` 的调度组</li>
 <li>如果为 `NULL`（默认），使用一个私有调度组</li>
 </ul>
 */
@property (nonatomic, strong) dispatch_group_t completionGroup;

///---------------------------------
/// @name Working Around System Bugs - 处理系统 Bugs
///---------------------------------

/**
 Whether to attempt to retry creation of upload tasks for background sessions when initial call returns `nil`. `NO` by default.
 <ul>
 <li>如果初始调用返回 `nil`，是否为后台会话重新创建上传任务</li>
 <li>默认为 `NO`</li>
 </ul>
 
 bug: As of iOS 7.0, there is a bug where upload tasks created for background tasks are sometimes `nil`. As a workaround, if this property is `YES`, AFNetworking will follow Apple's recommendation to try creating the task again.
 <p><strong>BUG</strong></p>
 <ul>
 <li>在 iOS 7.0中，有时会无法为后台任务创建上传任务，而只是得到 `nil`</li>
 <li>作为解决办法，如果将此属性设置为 `YES`，AFNetworking 将按照苹果的建议，尝试再次创建任务</li>
 </ul>

 @see https://github.com/AFNetworking/AFNetworking/issues/1675
 */
@property (nonatomic, assign) BOOL attemptsToRecreateUploadTasksForBackgroundSessions;

///---------------------
/// @name Initialization - 初始化
///---------------------

/**
 Creates and returns a manager for a session created with the specified configuration. This is the designated initializer.
 <ul>
 <li>使用指定的配置为网络会话创建并返回一个管理器</li>
 <li>这是指定的初始化方法</li>
 </ul>
 @param configuration The configuration used to create the managed session.
            <br />用于创建管理的网络会话的配置

 @return A manager for a newly-created session.
            <br />新创建的网络会话管理器
 */
- (instancetype)initWithSessionConfiguration:(NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

/**
 Invalidates the managed session, optionally canceling pending tasks.
 <ul>
 <li>使管理的会话无效</li>
 <li>可用于取消尚未完成的任务</li>
 </ul>

 @param cancelPendingTasks Whether or not to cancel pending tasks.
            <br />是否取消尚未完成的任务
 */
- (void)invalidateSessionCancelingTasks:(BOOL)cancelPendingTasks;

///-------------------------
/// @name Running Data Tasks - 运行数据任务
///-------------------------

/**
 Creates an `NSURLSessionDataTask` with the specified request.
 <br />使用指定的请求创建一个 `NSURLSessionDataTask`

 @param request The HTTP request for the request.
            <br />HTTP 请求
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，序列化器创建的响应对象，错误(如果有)</li>
 </ul>
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                            completionHandler:(void (^)(NSURLResponse *response, id responseObject, NSError *error))completionHandler;

///---------------------------
/// @name Running Upload Tasks - 运行上传任务
///---------------------------

/**
 Creates an `NSURLSessionUploadTask` with the specified request for a local file.
 <br />使用指定的请求和一个本地文件，创建一个 `NSURLSessionUploadTask`

 @param request The HTTP request for the request.
            <br />HTTP 请求
 @param fileURL A URL to the local file to be uploaded.
            <br />要上传的本地文件的 URL
 @param progress A progress object monitoring the current upload progress.
            <br />监听当前上传进度的进度对象
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，序列化器创建的响应对象，错误(如果有)</li>
 </ul>

 @see `attemptsToRecreateUploadTasksForBackgroundSessions`
 */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromFile:(NSURL *)fileURL
                                         progress:(NSProgress * __autoreleasing *)progress
                                completionHandler:(void (^)(NSURLResponse *response, id responseObject, NSError *error))completionHandler;

/**
 Creates an `NSURLSessionUploadTask` with the specified request for an HTTP body.
 <br />使用指定的请求和二进制数据，创建一个 `NSURLSessionUploadTask`

 @param request The HTTP request for the request.
            <br />HTTP 请求
 @param bodyData A data object containing the HTTP body to be uploaded.
            <br />要上传的 NSData 对象
 @param progress A progress object monitoring the current upload progress.
            <br />监听当前上传进度的进度对象
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，序列化器创建的响应对象，错误(如果有)</li>
 </ul>
 */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromData:(NSData *)bodyData
                                         progress:(NSProgress * __autoreleasing *)progress
                                completionHandler:(void (^)(NSURLResponse *response, id responseObject, NSError *error))completionHandler;

/**
 Creates an `NSURLSessionUploadTask` with the specified streaming request.
 <br />使用指定的流请求，创建一个 `NSURLSessionUploadTask`
 
 @param request The HTTP request for the request.
            <br />HTTP 请求
 @param progress A progress object monitoring the current upload progress.
            <br />监听当前上传进度的进度对象
 @param completionHandler A block object to be executed when the task finishes. This block has no return value and takes three arguments: the server response, the response object created by that serializer, and the error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，序列化器创建的响应对象，错误(如果有)</li>
 </ul>
 */
- (NSURLSessionUploadTask *)uploadTaskWithStreamedRequest:(NSURLRequest *)request
                                                 progress:(NSProgress * __autoreleasing *)progress
                                        completionHandler:(void (^)(NSURLResponse *response, id responseObject, NSError *error))completionHandler;

///-----------------------------
/// @name Running Download Tasks - 运行下载任务
///-----------------------------

/**
 Creates an `NSURLSessionDownloadTask` with the specified request.
 <br />使用指定的请求，创建一个 `NSURLSessionDownloadTask`

 @param request The HTTP request for the request.
            <br />HTTP 请求
 @param progress A progress object monitoring the current download progress.
            <br />监听当前下载进度的进度对象
 @param destination A block object to be executed in order to determine the destination of the downloaded file. This block takes two arguments, the target path & the server response, and returns the desired file URL of the resulting download. The temporary file used during the download will be automatically deleted after being moved to the returned URL.
 <ul>
 <li>确定保存下载文件目标位置的 block 对象</li>
 <li>该 block 有两个参数：目标路径和服务器响应</li>
 <li>返回保存下载文件目标位置的 URL</li>
 <li>当将下载的临时文件移动至返回的 URL 后，临时文件会被自动删除</li>
 </ul>
 @param completionHandler A block to be executed when a task finishes. This block has no return value and takes three arguments: the server response, the path of the downloaded file, and the error describing the network or parsing error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，下载的文件路径，错误(如果有)</li>
 </ul>

 @warning If using a background `NSURLSessionConfiguration` on iOS, these blocks will be lost when the app is terminated. Background sessions may prefer to use `-setDownloadTaskDidFinishDownloadingBlock:` to specify the URL for saving the downloaded file, rather than the destination block of this method.
 <br />如果使用 iOS 后台的 `NSURLSessionConfiguration`，应用程序被终止后，这些 block 都将会丢失。针对后台会话建议使用 `-setDownloadTaskDidFinishDownloadingBlock:` 指定保存下载文件的 URL，而不要使用此方法的 destination block
 */
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request
                                             progress:(NSProgress * __autoreleasing *)progress
                                          destination:(NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                    completionHandler:(void (^)(NSURLResponse *response, NSURL *filePath, NSError *error))completionHandler;

/**
 Creates an `NSURLSessionDownloadTask` with the specified resume data.
 <br />使用指定的续传数据，创建一个 `NSURLSessionDownloadTask`
 
 @param resumeData The data used to resume downloading.
            <br />用于断点续传的续传数据
 @param progress A progress object monitoring the current download progress.
            <br />监听当前下载进度的进度对象
 @param destination A block object to be executed in order to determine the destination of the downloaded file. This block takes two arguments, the target path & the server response, and returns the desired file URL of the resulting download. The temporary file used during the download will be automatically deleted after being moved to the returned URL.
 <ul>
 <li>确定保存下载文件目标位置的 block 对象</li>
 <li>该 block 有两个参数：目标路径和服务器响应</li>
 <li>返回保存下载文件目标位置的 URL</li>
 <li>当将下载的临时文件移动至返回的 URL 后，临时文件会被自动删除</li>
 </ul>
 @param completionHandler A block to be executed when a task finishes. This block has no return value and takes three arguments: the server response, the path of the downloaded file, and the error describing the network or parsing error that occurred, if any.
 <ul>
 <li>任务完成后执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有三个参数：服务器响应，下载的文件路径，错误(如果有)</li>
 </ul>
 */
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData
                                                progress:(NSProgress * __autoreleasing *)progress
                                             destination:(NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                       completionHandler:(void (^)(NSURLResponse *response, NSURL *filePath, NSError *error))completionHandler;

///---------------------------------
/// @name Getting Progress for Tasks - 获得任务进度
///---------------------------------

/**
 Returns the upload progress of the specified task.
 <br />返回指定任务的上传进度

 @param uploadTask The session upload task. Must not be `nil`.
            <br />会话的上传任务，不能为空

 @return An `NSProgress` object reporting the upload progress of a task, or `nil` if the progress is unavailable.
 <br />一个 `NSProgress` 对象报告任务的上传进度，如果进度不可用，返回 `nil`
 */
- (NSProgress *)uploadProgressForTask:(NSURLSessionUploadTask *)uploadTask;

/**
 Returns the download progress of the specified task.
 <br />返回指定任务的下载进度

 @param downloadTask The session download task. Must not be `nil`.
            <br />会话的下载任务，不能为空

 @return An `NSProgress` object reporting the download progress of a task, or `nil` if the progress is unavailable.
 <br />一个 `NSProgress` 对象报告任务的下载进度，如果进度不可用，返回 `nil`
 */
- (NSProgress *)downloadProgressForTask:(NSURLSessionDownloadTask *)downloadTask;

///-----------------------------------------
/// @name Setting Session Delegate Callbacks - 设置会话代理回调
///-----------------------------------------

/**
 Sets a block to be executed when the managed session becomes invalid, as handled by the `NSURLSessionDelegate` method `URLSession:didBecomeInvalidWithError:`.
 <ul>
 <li>设置当管理的会话无效时执行的 block</li>
 <li>在 `NSURLSessionDelegate` 的 `URLSession:didBecomeInvalidWithError:` 方法中执行</li>
 </ul>
 
 @param block A block object to be executed when the managed session becomes invalid. The block has no return value, and takes two arguments: the session, and the error related to the cause of invalidation.
 <ul>
 <li>当管理的会话无效时执行的 block 对象</li>
 <li>该 block 没有返回值</li>
 <li>该 block 有两个参数：会话和导致会话无效相关的错误</li>
 </ul>
 */
- (void)setSessionDidBecomeInvalidBlock:(void (^)(NSURLSession *session, NSError *error))block;

/**
 Sets a block to be executed when a connection level authentication challenge has occurred, as handled by the `NSURLSessionDelegate` method `URLSession:didReceiveChallenge:completionHandler:`.
 <ul>
 <li>设置当连接需要身份验证质询时执行的 block</li>
 <li>在 `NSURLSessionDelegate` 的 `URLSession:didReceiveChallenge:completionHandler:` 方法中执行</li>
 </ul>

 @param block A block object to be executed when a connection level authentication challenge has occurred. The block returns the disposition of the authentication challenge, and takes three arguments: the session, the authentication challenge, and a pointer to the credential that should be used to resolve the challenge.
 <ul>
 <li>当连接需要身份验证质询时执行的 block 对象</li>
 <li>该 block 返回身份验证的配置情况</li>
 <li>该 block 有三个参数：会话，身份验证质询和一个指向凭据的指针，该凭据用于解决身份验证</li>
 </ul>
 */
- (void)setSessionDidReceiveAuthenticationChallengeBlock:(NSURLSessionAuthChallengeDisposition (^)(NSURLSession *session, NSURLAuthenticationChallenge *challenge, NSURLCredential * __autoreleasing *credential))block;

///--------------------------------------
/// @name Setting Task Delegate Callbacks - 设置任务代理回调
///--------------------------------------

/**
 Sets a block to be executed when a task requires a new request body stream to send to the remote server, as handled by the `NSURLSessionTaskDelegate` method `URLSession:task:needNewBodyStream:`.

 @param block A block object to be executed when a task requires a new request body stream.
 */
- (void)setTaskNeedNewBodyStreamBlock:(NSInputStream * (^)(NSURLSession *session, NSURLSessionTask *task))block;

/**
 Sets a block to be executed when an HTTP request is attempting to perform a redirection to a different URL, as handled by the `NSURLSessionTaskDelegate` method `URLSession:willPerformHTTPRedirection:newRequest:completionHandler:`.

 @param block A block object to be executed when an HTTP request is attempting to perform a redirection to a different URL. The block returns the request to be made for the redirection, and takes four arguments: the session, the task, the redirection response, and the request corresponding to the redirection response.
 */
- (void)setTaskWillPerformHTTPRedirectionBlock:(NSURLRequest * (^)(NSURLSession *session, NSURLSessionTask *task, NSURLResponse *response, NSURLRequest *request))block;

/**
 Sets a block to be executed when a session task has received a request specific authentication challenge, as handled by the `NSURLSessionTaskDelegate` method `URLSession:task:didReceiveChallenge:completionHandler:`.

 @param block A block object to be executed when a session task has received a request specific authentication challenge. The block returns the disposition of the authentication challenge, and takes four arguments: the session, the task, the authentication challenge, and a pointer to the credential that should be used to resolve the challenge.
 */
- (void)setTaskDidReceiveAuthenticationChallengeBlock:(NSURLSessionAuthChallengeDisposition (^)(NSURLSession *session, NSURLSessionTask *task, NSURLAuthenticationChallenge *challenge, NSURLCredential * __autoreleasing *credential))block;

/**
 Sets a block to be executed periodically to track upload progress, as handled by the `NSURLSessionTaskDelegate` method `URLSession:task:didSendBodyData:totalBytesSent:totalBytesExpectedToSend:`.

 @param block A block object to be called when an undetermined number of bytes have been uploaded to the server. This block has no return value and takes five arguments: the session, the task, the number of bytes written since the last time the upload progress block was called, the total bytes written, and the total bytes expected to be written during the request, as initially determined by the length of the HTTP body. This block may be called multiple times, and will execute on the main thread.
 */
- (void)setTaskDidSendBodyDataBlock:(void (^)(NSURLSession *session, NSURLSessionTask *task, int64_t bytesSent, int64_t totalBytesSent, int64_t totalBytesExpectedToSend))block;

/**
 Sets a block to be executed as the last message related to a specific task, as handled by the `NSURLSessionTaskDelegate` method `URLSession:task:didCompleteWithError:`.

 @param block A block object to be executed when a session task is completed. The block has no return value, and takes three arguments: the session, the task, and any error that occurred in the process of executing the task.
 */
- (void)setTaskDidCompleteBlock:(void (^)(NSURLSession *session, NSURLSessionTask *task, NSError *error))block;

///-------------------------------------------
/// @name Setting Data Task Delegate Callbacks - 设置数据任务代理回调
///-------------------------------------------

/**
 Sets a block to be executed when a data task has received a response, as handled by the `NSURLSessionDataDelegate` method `URLSession:dataTask:didReceiveResponse:completionHandler:`.

 @param block A block object to be executed when a data task has received a response. The block returns the disposition of the session response, and takes three arguments: the session, the data task, and the received response.
 */
- (void)setDataTaskDidReceiveResponseBlock:(NSURLSessionResponseDisposition (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSURLResponse *response))block;

/**
 Sets a block to be executed when a data task has become a download task, as handled by the `NSURLSessionDataDelegate` method `URLSession:dataTask:didBecomeDownloadTask:`.

 @param block A block object to be executed when a data task has become a download task. The block has no return value, and takes three arguments: the session, the data task, and the download task it has become.
 */
- (void)setDataTaskDidBecomeDownloadTaskBlock:(void (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSURLSessionDownloadTask *downloadTask))block;

/**
 Sets a block to be executed when a data task receives data, as handled by the `NSURLSessionDataDelegate` method `URLSession:dataTask:didReceiveData:`.

 @param block A block object to be called when an undetermined number of bytes have been downloaded from the server. This block has no return value and takes three arguments: the session, the data task, and the data received. This block may be called multiple times, and will execute on the session manager operation queue.
 */
- (void)setDataTaskDidReceiveDataBlock:(void (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSData *data))block;

/**
 Sets a block to be executed to determine the caching behavior of a data task, as handled by the `NSURLSessionDataDelegate` method `URLSession:dataTask:willCacheResponse:completionHandler:`.

 @param block A block object to be executed to determine the caching behavior of a data task. The block returns the response to cache, and takes three arguments: the session, the data task, and the proposed cached URL response.
 */
- (void)setDataTaskWillCacheResponseBlock:(NSCachedURLResponse * (^)(NSURLSession *session, NSURLSessionDataTask *dataTask, NSCachedURLResponse *proposedResponse))block;

/**
 Sets a block to be executed once all messages enqueued for a session have been delivered, as handled by the `NSURLSessionDataDelegate` method `URLSessionDidFinishEventsForBackgroundURLSession:`.

 @param block A block object to be executed once all messages enqueued for a session have been delivered. The block has no return value and takes a single argument: the session.
 */
- (void)setDidFinishEventsForBackgroundURLSessionBlock:(void (^)(NSURLSession *session))block;

///-----------------------------------------------
/// @name Setting Download Task Delegate Callbacks - 设置下载任务代理回调
///-----------------------------------------------

/**
 Sets a block to be executed when a download task has completed a download, as handled by the `NSURLSessionDownloadDelegate` method `URLSession:downloadTask:didFinishDownloadingToURL:`.

 @param block A block object to be executed when a download task has completed. The block returns the URL the download should be moved to, and takes three arguments: the session, the download task, and the temporary location of the downloaded file. If the file manager encounters an error while attempting to move the temporary file to the destination, an `AFURLSessionDownloadTaskDidFailToMoveFileNotification` will be posted, with the download task as its object, and the user info of the error.
 */
- (void)setDownloadTaskDidFinishDownloadingBlock:(NSURL * (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, NSURL *location))block;

/**
 Sets a block to be executed periodically to track download progress, as handled by the `NSURLSessionDownloadDelegate` method `URLSession:downloadTask:didWriteData:totalBytesWritten:totalBytesWritten:totalBytesExpectedToWrite:`.

 @param block A block object to be called when an undetermined number of bytes have been downloaded from the server. This block has no return value and takes five arguments: the session, the download task, the number of bytes read since the last time the download progress block was called, the total bytes read, and the total bytes expected to be read during the request, as initially determined by the expected content size of the `NSHTTPURLResponse` object. This block may be called multiple times, and will execute on the session manager operation queue.
 */
- (void)setDownloadTaskDidWriteDataBlock:(void (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, int64_t bytesWritten, int64_t totalBytesWritten, int64_t totalBytesExpectedToWrite))block;

/**
 Sets a block to be executed when a download task has been resumed, as handled by the `NSURLSessionDownloadDelegate` method `URLSession:downloadTask:didResumeAtOffset:expectedTotalBytes:`.

 @param block A block object to be executed when a download task has been resumed. The block has no return value and takes four arguments: the session, the download task, the file offset of the resumed download, and the total number of bytes expected to be downloaded.
 */
- (void)setDownloadTaskDidResumeBlock:(void (^)(NSURLSession *session, NSURLSessionDownloadTask *downloadTask, int64_t fileOffset, int64_t expectedTotalBytes))block;

@end

#endif

///--------------------
/// @name Notifications - 通知
///--------------------

/**
 Posted when a task begins executing.
 <br />任务开始执行通知

 @deprecated Use `AFNetworkingTaskDidResumeNotification` instead.
 <br />已经废弃，使用 `AFNetworkingTaskDidResumeNotification` 替代
 */
extern NSString * const AFNetworkingTaskDidStartNotification DEPRECATED_ATTRIBUTE;

/**
 Posted when a task resumes.
 <br />任务继续通知
 */
extern NSString * const AFNetworkingTaskDidResumeNotification;

/**
 Posted when a task finishes executing. Includes a userInfo dictionary with additional information about the task.
 <br />任务结束执行通知，任务附加信息包含在 userInfo 字典中

 @deprecated Use `AFNetworkingTaskDidCompleteNotification` instead.
 <br />已经废弃，使用 `AFNetworkingTaskDidCompleteNotification` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishNotification DEPRECATED_ATTRIBUTE;

/**
 Posted when a task finishes executing. Includes a userInfo dictionary with additional information about the task.
 <br />任务结束执行通知，任务附加信息包含在 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteNotification;

/**
 Posted when a task suspends its execution.
 <br />任务暂停执行通知
 */
extern NSString * const AFNetworkingTaskDidSuspendNotification;

/**
 Posted when a session is invalidated.
 <br />网络会话无效通知
 */
extern NSString * const AFURLSessionDidInvalidateNotification;

/**
 Posted when a session download task encountered an error when moving the temporary download file to a specified destination.
 <br />会话的下载任务将临时下载文件移动指定目录发生错误通知
 */
extern NSString * const AFURLSessionDownloadTaskDidFailToMoveFileNotification;

/**
 The raw response data of the task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if response data exists for the task.

 @deprecated Use `AFNetworkingTaskDidCompleteResponseDataKey` instead.
 <br />请使用 `AFNetworkingTaskDidCompleteResponseDataKey` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishResponseDataKey DEPRECATED_ATTRIBUTE;

/**
 The raw response data of the task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if response data exists for the task.
 <br />任务的原始响应数据，如果任务存在响应数据，包含在 `AFNetworkingTaskDidFinishNotification` 的 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteResponseDataKey;

/**
 The serialized response object of the task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if the response was serialized.

 @deprecated Use `AFNetworkingTaskDidCompleteSerializedResponseKey` instead.
 <br />请使用 `AFNetworkingTaskDidCompleteSerializedResponseKey` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishSerializedResponseKey DEPRECATED_ATTRIBUTE;

/**
 The serialized response object of the task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if the response was serialized.
 <br />任务响应数据的反序列化对象，如果响应数据已经被反序列化，包含在 `AFNetworkingTaskDidFinishNotification` 的 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteSerializedResponseKey;

/**
 The response serializer used to serialize the response. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if the task has an associated response serializer.

 @deprecated Use `AFNetworkingTaskDidCompleteResponseSerializerKey` instead.
 <br />请使用 `AFNetworkingTaskDidCompleteResponseSerializerKey` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishResponseSerializerKey DEPRECATED_ATTRIBUTE;

/**
 The response serializer used to serialize the response. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if the task has an associated response serializer.
 <br />响应序列化器用于反序列化响应数据，如果任务已经关联至响应序列化器，包含在 `AFNetworkingTaskDidFinishNotification` 的 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteResponseSerializerKey;

/**
 The file path associated with the download task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if an the response data has been stored directly to disk.

 @deprecated Use `AFNetworkingTaskDidCompleteAssetPathKey` instead.
 <br />请使用 `AFNetworkingTaskDidCompleteAssetPathKey` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishAssetPathKey DEPRECATED_ATTRIBUTE;

/**
 The file path associated with the download task. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if an the response data has been stored directly to disk.
 <br />与下载任务关联的文件路径，如果响应数据已经被保存至磁盘，包含在 `AFNetworkingTaskDidFinishNotification` 的 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteAssetPathKey;

/**
 Any error associated with the task, or the serialization of the response. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if an error exists.

 @deprecated Use `AFNetworkingTaskDidCompleteErrorKey` instead.
 <br />请使用 `AFNetworkingTaskDidCompleteErrorKey` 替代
 */
extern NSString * const AFNetworkingTaskDidFinishErrorKey DEPRECATED_ATTRIBUTE;

/**
 Any error associated with the task, or the serialization of the response. Included in the userInfo dictionary of the `AFNetworkingTaskDidFinishNotification` if an error exists.
 <br />与任务或者响应反序列化关联的错误，如果错误存在，包括在 `AFNetworkingTaskDidFinishNotification` 的 userInfo 字典中
 */
extern NSString * const AFNetworkingTaskDidCompleteErrorKey;
