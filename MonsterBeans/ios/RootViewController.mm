//
//  MonsterBeansAppController.h
//  MonsterBeans
//
//  Created by A on 3/7/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//

#import "RootViewController.h"
#import "AppDelegate.h"
#import "AppController.h"
#import "EAGLView.h"
using namespace cocos2d;
@implementation RootViewController


- (void)dealloc
{
    [super dealloc];
}

- (void)loadView
{
    AppController *appDelegate = (AppController *)[UIApplication sharedApplication].delegate;
    
    EAGLView *view = [EAGLView viewWithFrame:[appDelegate.window bounds]
                                 pixelFormat:kEAGLColorFormatRGBA8
                                 depthFormat:GL_DEPTH_COMPONENT16
                          preserveBackbuffer:NO
                                  sharegroup:nil
                               multiSampling:NO
                             numberOfSamples:0];
    
    self.view = view;
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    CGRect rect = [[UIScreen mainScreen] bounds];
    CGSize size = rect.size;
    CGFloat height = size.height;
    
    }


- (UIViewController *)viewControllerToPresent
{
    return self;
}

- (UIViewController *)viewControllerForPresentingModalView
{
    return self;
}






- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return UIInterfaceOrientationIsPortrait(toInterfaceOrientation);
}



@end
