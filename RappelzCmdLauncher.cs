using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using HANDLE = System.IntPtr;

namespace RappelzCmdLauncher
{
	class RappelzCmdLauncher
	{
		[DllImport("Kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.Winapi, CharSet = CharSet.Auto)]
        public static extern HANDLE CreateEvent(HANDLE lpEventAttributes, [In, MarshalAs(UnmanagedType.Bool)] bool bManualReset, [In, MarshalAs(UnmanagedType.Bool)] bool bIntialState, [In, MarshalAs(UnmanagedType.BStr)] string lpName);

		[DllImport("Kernel32.dll", SetLastError = true)]
		static extern UInt32 WaitForSingleObject(IntPtr hHandle, UInt32 dwMilliseconds);

		[StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES {
            public int nLength;
            public IntPtr lpSecurityDescriptor;
            public int bInheritHandle;
        }

		static IntPtr securityAttributeToPtr(SECURITY_ATTRIBUTES attrib) {
			int sz = Marshal.SizeOf(attrib.GetType());
			IntPtr ptr = Marshal.AllocHGlobal(sz);
			Marshal.StructureToPtr(attrib, ptr, false);

			return ptr;
		}

		static HANDLE createEvent()
		{
			SECURITY_ATTRIBUTES securityAttrib;

			securityAttrib.bInheritHandle = 1;
			securityAttrib.lpSecurityDescriptor = IntPtr.Zero;
			securityAttrib.nLength = Marshal.SizeOf(typeof(SECURITY_ATTRIBUTES));

			IntPtr securityAttribPtr = securityAttributeToPtr(securityAttrib);
			return CreateEvent(securityAttribPtr, false, false, String.Empty);
		}

		static void Main(string[] args)
		{
			if (args.Length < 2)
				return;

			ProcessStartInfo startInfo = new ProcessStartInfo();
			HANDLE eventHandle = createEvent();


			string sframeFilename = Path.GetFileName(args[0]);
			startInfo.EnvironmentVariables[String.Format("{0}_PARENT", sframeFilename)] = "Launcher.exe";
			startInfo.EnvironmentVariables[String.Format("{0}_RUNNER", sframeFilename)] = eventHandle.ToString();
			startInfo.UseShellExecute = false;
			startInfo.FileName = args[0];
			startInfo.Arguments = String.Join(" ", args.Skip(1).Select(x => "\"" + x + "\""));

			Process.Start(startInfo);

			WaitForSingleObject(eventHandle, 10 * 1000);
		}
	}
}
