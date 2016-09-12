using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web.Http;
using System.Web.Http.Description;
using JDataService.DataService;
using JDataService.Models;
using System.Text;

namespace JDataService.Controllers
{
    public class LedController : ApiController
    {
        private LabRunContext db = new LabRunContext();


        // GET: api/led
        [ResponseType(typeof(LabRunState))]
        public async Task<IHttpActionResult> GetLedData()
        {
            var labRunData = (await db.GetLabRunResults()).ToList();

            var ledData = new LabRunState();
            if (labRunData.Count() > 0)
                ledData.LedColor = "red";
            else
                ledData.LedColor = "green";

            StringBuilder message = new StringBuilder();
            bool firstMessage = true;
            foreach (var lab in labRunData)
            {
                if (!firstMessage)
                    message.Append(" -- ");
                message.Append(String.Format("{0} Inv {1}, Fail {2}, Prob {3}", lab.FriendlyName, lab.InvestigateJobcount , lab.TestFailJobCount , lab.ProblemJobCount));
                firstMessage = false;
            }
            //ledData.Message = message.ToString();
            ledData.Message = "m";

            if (ledData == null)
            {
                return NotFound();
            }

            return Ok(ledData);
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }

    }
}