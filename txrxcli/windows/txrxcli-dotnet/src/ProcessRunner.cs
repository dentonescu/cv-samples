using System.Diagnostics;

namespace TxRxCli
{
    internal static class ProcessRunner
    {
        public static ProcessStartInfo BuildProcessStartInfo(string shell, string command)
        {
            var psi = new ProcessStartInfo
            {
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            if (string.Equals(shell, Protocol.ShellCmd, StringComparison.OrdinalIgnoreCase))
            {
                psi.FileName = "cmd.exe";
                psi.Arguments = $"/C {command}";
            }
            else if (string.Equals(shell, Protocol.ShellPowershell, StringComparison.OrdinalIgnoreCase))
            {
                psi.FileName = "powershell.exe";
                psi.Arguments = $"-NoProfile -Command \"{command}\"";
            }
            else
            {
                throw new InvalidOperationException($"Unknown shell: {shell}");
            }

            return psi;
        }

        public static (int exitCode, string stdout, string stderr) Run(string shell, string command)
        {
            ProcessStartInfo psi = BuildProcessStartInfo(shell, command);

            using var proc = Process.Start(psi);
            if (proc is null)
            {
                return (-1, string.Empty, "failed to start process");
            }

            string stdout = proc.StandardOutput.ReadToEnd();
            string stderr = proc.StandardError.ReadToEnd();
            proc.WaitForExit();
            return (proc.ExitCode, stdout, stderr);
        }
    }
}
