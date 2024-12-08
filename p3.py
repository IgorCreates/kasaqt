# ##!/home/juka/Rad/KasaQT/kasa-python/bin/python3
import os
import argparse
import json
import logging
from fiskalhr.invoice import Invoice
from fiskalhr.ws import FiskalClient
from fiskalhr.signature import Signer, Verifier
from datetime import datetime
from timeit import default_timer as timer


VER = "2.0.0"

# print(os.getcwd())
parser = argparse.ArgumentParser(
    prog="fiskal.py", description="wrapper for fiskal-hr "
)
parser.add_argument(
    'file',
    metavar='FILE',
    type=str,
    help="Specify the Invoice JSON file to process."
)
parser.add_argument(
    "-z", "--zki",action='store_true', help="Calculate ZKI"
)
parser.add_argument(
    "-s", "--send",action='store_true', help="submit invoice to Fina"
)
args = parser.parse_args()

if __name__ == "__main__":
    timer_start = timer()
    args = parser.parse_args()
    try:
        with open(args.file, 'r') as file:
            invJson = json.load(file)
    except FileNotFoundError:
        print(f"Error: File {args.file} not found.")
        parser.print_help()
        exit(1)

    logging.basicConfig(
        filename=args.file + ".log",
        filemode="a",
        format="%(asctime)s - %(name)s - %(levelname)s -  %(message)s",
        level=os.environ.get("LOGLEVEL", "INFO"),
    )
    logging.getLogger("fiskal").setLevel(level=os.environ.get("LOGLEVEL", "INFO"))
    logs = logging.getLogger("fiskal")
    logs.info("Starting")
    if os.environ.get("LOGLEVEL", "INFO") == "DEBUG":
        import xmlsec
        xmlsec.enable_debug_trace(True)


    s = Signer("./certificate.pem", "./passkey.pem","SupperPassword")
    v = Verifier("./certs/prodCAfile.pem",["./certs/prodCAfile.pem"])
    fc = FiskalClient("./certs/prodCAfile.pem","./wsdl/FiskalizacijaService.wsdl",s,v)
#    fc.test_service()
    inv = Invoice(fc)
    inv.oib = invJson['Oib']
    inv.operator_oib = invJson['OibOper']
    inv.invoice_number = f"{invJson['BrRac']['BrOznRac']}/{invJson['BrRac']['OznPosPr']}/{invJson['BrRac']['OznNapUr']}"
    inv.issued_at = datetime.strptime(invJson['DatVrijeme'], "%d.%m.%YT%H:%M:%S")
    inv.total = invJson['IznosUkupno']
    inv.payment_method = invJson['NacinPlac']
    inv.is_late_registration = invJson['NakDost']
    inv.sequence_scope = invJson['OznSlijed']
    inv.is_vat_registered = invJson['USustPdv']

    if args.zki:
        zki = inv.calculate_zki()
        print(f"ZKI: {zki}")
        logs.debug("ZKI: %s", zki)
    if args.send:
        zki = inv.calculate_zki()
        print(f"ZKI: {zki}")
        logs.debug("ZKI: %s", zki)
        qr_link = inv.get_qr_link()
        print(f"QR_LINK: {qr_link}")
        logs.debug("QR_LINK: %s", qr_link)
        # jir = inv.submit_invoice()
        # print(f"JIR: {jir}")
    print("VER:" + VER)
    logs.debug("VER: %s", VER)
    # print("DoneIn:" + str(timer() - timer_start) + "seconds")
    print(f"DoneIn: {timer() - timer_start:.4f} seconds")
    logs.info(f"DoneIn: {timer() - timer_start:.4f} seconds")


