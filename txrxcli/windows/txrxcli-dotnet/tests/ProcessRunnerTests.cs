using System;
using System.Diagnostics;
using System.IO;
using Xunit;
using Xunit.Abstractions;
using static TxRxCli.ProcessRunner;

namespace TxRxCli.Tests
{
    public class ProcessRunnerTests
    {
        public ProcessRunnerTests(ITestOutputHelper output) => _output = output;

        private readonly ITestOutputHelper _output;

        private string Describe(ProcessStartInfo psi) =>
            $"ProcessStartInfo: FileName={psi.FileName} ; Arguments={psi.Arguments}".Trim();


        [Fact]
        public void Cmd_Succeeds()
        {
            var psi = ProcessRunner.BuildProcessStartInfo("cmd", "echo something");
            Assert.NotNull(psi);
            Assert.Equal("cmd.exe", psi.FileName);
            Assert.Equal("/C echo something", psi.Arguments);
            _output.WriteLine(Describe(psi));
        }

        [Fact]
        public void PowerShell_Succeeds()
        {
            var psi = ProcessRunner.BuildProcessStartInfo("powershell", "Write-Host blah");
            Assert.NotNull(psi);
            Assert.Equal("powershell.exe", psi.FileName);
            Assert.Equal("-NoProfile -Command \"Write-Host blah\"", psi.Arguments);
            _output.WriteLine(Describe(psi));
        }

        [Fact]
        public void UnknownShell_Throws()
        {
            Assert.Throws<InvalidOperationException>(
                () => ProcessRunner.BuildProcessStartInfo("sh", "echo hi")
            );
        }

    }
}
