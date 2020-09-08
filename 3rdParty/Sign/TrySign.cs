using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;


namespace Sign
{
    class TrySign
    {
        private string _certificatePath;
        private string _certificatePassword;
        private string _stringToSign;

        public TrySign(string certificatePath, string certificatePassword, string stringToSign)
        {
            this._certificatePath = certificatePath;
            this._certificatePassword = certificatePassword;
            this._stringToSign = stringToSign;
        }

        public string MakePotpis()
        {
            //string zaPotpis;
            //zaPotpis = CreateKodStart();
            return Potpisi(this._stringToSign);
        }

        private string CreateKodStart()
        {
            // početak
            // pročitaj (oib)
            string oib = "00169331406";
            // medjurezultat = oib
            string medjurezultat = oib;

            // pročitaj (datVrij – datum i vrijeme izdavanja računa zapisani kao tekst u formatu 'dd.MM.gggg HH:mm:ss')
            string datVrij = "01.10.2012 16:04:25";
            // medjurezultat = medjurezultat + datVrij
            medjurezultat = medjurezultat + datVrij;
            // pročitaj (bor – brojčana oznaka računa)
            string bor = "12345";
            // medjurezultat = medjurezultat + bor
            medjurezultat = medjurezultat + bor;
            // pročitaj (opp – oznaka poslovnog prostora)
            string opp = "blag001";
            // medjurezultat = medjurezultat + opp
            medjurezultat = medjurezultat + opp;
            // pročitaj (onu – oznaka naplatnog uređaja)
            string onu = "11245";
            // medjurezultat = medjurezultat + onu
            medjurezultat = medjurezultat + onu;
            // pročitaj (uir - ukupni iznos računa)
            string uir = "1245.56";
            // medjurezultat = medjurezultat + uir
            medjurezultat = medjurezultat + uir;
            // elektronički potpiši medjurezultat koristeći RSA-SHA1 potpis
            return medjurezultat;
        }

        private string Potpisi(string medjurezultat)
        {
            byte[] potpisano = null;
            try
            {
                X509Certificate2 certifikat = new X509Certificate2(this._certificatePath, this._certificatePassword);
                RSACryptoServiceProvider rsa = (RSACryptoServiceProvider)certifikat.PrivateKey;
                byte[] podaci = Encoding.ASCII.GetBytes(medjurezultat);
                potpisano = rsa.SignData(podaci, new SHA1CryptoServiceProvider());
                // rezultatIspis = izračunajMD5(elektronički potpisani medjurezultat)
                MD5 md5Hash = MD5.Create();
                string rezultatIspis = GetMd5Hash(md5Hash, potpisano);
                // kraj
                //Console.WriteLine("Dobiveni 32-znamenkasti zaštitni kod je: " + rezultatIspis);
                return rezultatIspis;
            }
            catch (Exception ex)
            {
                // greška
                return "ERROR";
                //Console.WriteLine(ex.Message);
            }
        }

        // Metoda za izračun MD5 hash
        private string GetMd5Hash(MD5 md5Hash, byte[] input)
        {
            byte[] data = md5Hash.ComputeHash(input);
            StringBuilder sBuilder = new StringBuilder();
            for (int i = 0; i < data.Length; i++)
            {
                sBuilder.Append(data[i].ToString("x2"));
            }
            return sBuilder.ToString();
        }
    }
}
