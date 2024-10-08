/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "DS-MandatoryExercise/rpc.h"

bool_t
xdr_item (XDR *xdrs, item *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->value2))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->value3))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->error))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->num_items))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_set_value_1_argument (XDR *xdrs, set_value_1_argument *objp)
{
	 if (!xdr_int (xdrs, &objp->key))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->value2))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_modify_value_1_argument (XDR *xdrs, modify_value_1_argument *objp)
{
	 if (!xdr_int (xdrs, &objp->key))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->value1, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->value2))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->value3))
		 return FALSE;
	return TRUE;
}
