using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace JDataService.Models
{
    public class LedDataResult
    {
        public string ledColor { get; set; }
        public IQueryable<LabRunResult> labRunData { get; set; }
    }
}