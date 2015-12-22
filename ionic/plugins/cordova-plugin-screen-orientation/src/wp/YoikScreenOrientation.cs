/*
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

using System;
using System.Net;
using System.IO;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Notification;
using Microsoft.Phone.Shell;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Runtime.Serialization;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Threading;

using WPCordovaClassLib.Cordova;
using WPCordovaClassLib.Cordova.Commands;
using WPCordovaClassLib.Cordova.JSON;


namespace WPCordovaClassLib.Cordova.Commands
{
    public class YoikScreenOrientation : BaseCommand
    {
        #region Static members
        private const string UNLOCKED = "unlocked";

        private const string PORTRAIT = "portrait";
        private const string PORTRAIT_PRIMARY = "portrait-primary";
        private const string PORTRAIT_SECONDARY = "portrait-secondary";

        private const string LANDSCAPE = "landscape";
        private const string LANDSCAPE_PRIMARY = "landscape-primary";
        private const string LANDSCAPE_SECONDARY = "landscape-secondary";

        #endregion

        /// <summary>
        /// Current orientation
        /// </summary>
        private string currentOrientation;

        public YoikScreenOrientation()
        {

        }

        /// <summary>
        /// Changes the orientation
        /// </summary>

        public void screenOrientation(string options)
        {
            string orientation = null;
            try
            {
                orientation = JSON.JsonHelper.Deserialize<string[]>(options)[0];
            }
            catch (Exception ex)
            {
                this.DispatchCommandResult(new PluginResult(PluginResult.Status.JSON_EXCEPTION, ex.Message));
                return;
            }

            if (string.IsNullOrEmpty(orientation))
            {
                this.DispatchCommandResult(new PluginResult(PluginResult.Status.JSON_EXCEPTION));
                return;
            }

            if (this.currentOrientation != orientation) // Might prevent flickering
            {

                Deployment.Current.Dispatcher.BeginInvoke(() =>
                {
                    PhoneApplicationFrame frame;
                    PhoneApplicationPage page;
                    if (TryCast(Application.Current.RootVisual, out frame) &&
                      TryCast(frame.Content, out page))
                    {
                        if (orientation == PORTRAIT || orientation == PORTRAIT_PRIMARY || orientation == PORTRAIT_SECONDARY)
                        {
                            page.SupportedOrientations = SupportedPageOrientation.Portrait;
                        }

                        else if (orientation == LANDSCAPE || orientation == LANDSCAPE_PRIMARY || orientation == LANDSCAPE_SECONDARY)
                        {
                            page.SupportedOrientations = SupportedPageOrientation.Landscape;
                        }
                        else if (orientation == UNLOCKED)
                        {
                            page.SupportedOrientations = SupportedPageOrientation.PortraitOrLandscape;
                        }
                        else
                        {
                            this.DispatchCommandResult(new PluginResult(PluginResult.Status.ERROR, "Screen orientation not detected."));
                            return;
                        }
                        this.currentOrientation = orientation;
                    }
                });

                this.DispatchCommandResult();
            }
        }

        static bool TryCast<T>(object obj, out T result) where T : class
        {
            result = obj as T;
            return result != null;
        }
    }
}
