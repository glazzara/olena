<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="/"> <!-- "/" means the whole document (root) -->
    <html>
      <head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<title>SCRIBO</title>
	<link rel="stylesheet" type="text/css" href="css.css" />
      </head>
      <body>

	<xsl:for-each select="pcGts/page/graphic_region|
			      pcGts/page/image_region|
			      pcGts/page/chart_region|
			      pcGts/page/table_region|
			      pcGts/page/separator_region|
			      pcGts/page/text_region|
			      pcGts/page/text_region/paragraph|
			      pcGts/page/text_region/paragraph/line">

	  <!-- Regions Coordinates -->

	  <!-- y1 -->
	  <xsl:variable name="y1">
	    <xsl:for-each select="coords/point">
	      <xsl:sort select="@y" order="ascending" data-type="number"/>
	      <xsl:if test="position() = 1">
		<xsl:value-of select="@y" />
	      </xsl:if>
	    </xsl:for-each>
	  </xsl:variable>

	  <!-- x2 -->
	  <xsl:variable name="x2">
	    <xsl:for-each select="coords/point">
	      <xsl:sort select="@x" order="descending" data-type="number"/>
	      <xsl:if test="position() = 1">
		<xsl:value-of select="@x" />
	      </xsl:if>
	    </xsl:for-each>
	  </xsl:variable>

	  <!-- y2 -->
	  <xsl:variable name="y2">
	    <xsl:for-each select="coords/point">
	      <xsl:sort select="@y" order="descending" data-type="number"/>
	      <xsl:if test="position() = 1">
		<xsl:value-of select="@y" />
	      </xsl:if>
	    </xsl:for-each>
	  </xsl:variable>

	  <!-- x1 -->
	  <xsl:variable name="x1">
	    <xsl:for-each select="coords/point">
	      <xsl:sort select="@x" order="ascending" data-type="number"/>
	      <xsl:if test="position() = 1">
		<xsl:value-of select="@x" />
	      </xsl:if>
	    </xsl:for-each>
	  </xsl:variable>

	  <!-- END OF Regions Coordinates -->

	  <!-- TEXT LINE-->
	  <xsl:if test="name() = 'line'">

	    <xsl:variable name="colour">
	      <xsl:choose>
		<xsl:when test="../../@txt_colour != ''">
		  <xsl:value-of select="../../@txt_colour" />
		</xsl:when>
		<xsl:otherwise>
		  Black
		</xsl:otherwise>
	      </xsl:choose>
	    </xsl:variable>

	    <!-- x_height -->
	    <xsl:variable name="x_height">
	      <xsl:value-of select="../../@x_height" />
	    </xsl:variable>

	    <!-- a_height -->
	    <xsl:variable name="a_height">
	      <xsl:value-of select="../../@a_height" />
	    </xsl:variable>

	    <!-- ABS(d_height) -->
	    <xsl:variable name="d_height_abs">
	      <xsl:choose>
		<xsl:when test="../../@d_height &lt; 0">
		  <xsl:value-of select="-../../@d_height" />
		</xsl:when>
		<xsl:otherwise>
		  <xsl:value-of select="../../@d_height" />
		</xsl:otherwise>
	      </xsl:choose>
	    </xsl:variable>

	    <!-- a_height computed, i.e>
		 if (A - X < |D|)
		 A = X + |D|
	      -->
	    <xsl:variable name="a">
	      <xsl:choose>
		<xsl:when test="($a_height - $x_height) &lt; $d_height_abs">
		  <xsl:value-of select="$x_height + $d_height_abs" />
		</xsl:when>
		<xsl:otherwise>
		  <xsl:value-of select="$a_height" />
		</xsl:otherwise>
	      </xsl:choose>
	    </xsl:variable>

	    <!-- d_height computed, i.e>
		 if (A - A > |D|)
		 |D| = A - X
	      -->
	    <xsl:variable name="d">
	      <xsl:choose>
		<xsl:when test="($a_height - $x_height) &gt; $d_height_abs">
		  <xsl:value-of select="$a_height - $x_height" />
		</xsl:when>
		<xsl:otherwise>
		  <xsl:value-of select="$d_height_abs" />
		</xsl:otherwise>
	      </xsl:choose>
	    </xsl:variable>

            <div class="line" onmouseover="this.style.opacity=0.2;this.filters.alpha.opacity=20"
		 onmouseout="this.style.opacity=1;this.filters.alpha.opacity=100">
              <xsl:attribute name="style">
                opacity:1;
                height:auto;
                font-size:<xsl:value-of select="$a+$d" />px;
		width:<xsl:value-of select="$x2 - $x1" />px;
                left:<xsl:value-of select="$x1 " />px;
                top:<xsl:value-of select="$y1 " />px;
		color:<xsl:value-of select="$colour" />;
              </xsl:attribute>
	      <xsl:value-of select="@text"/>
	    </div>
	  </xsl:if>
	  <!-- ENF OF TEXT LINE -->

	  <!-- TEXT REGION -->
	  <xsl:if test="name() = 'text_region'">
	    <div class="region">
	      <xsl:attribute name="style">
		height:<xsl:value-of select="$y2 - $y1" />px;
		width:<xsl:value-of select="$x2 - $x1" />px;
		left:<xsl:value-of select="$x1" />px;
		top:<xsl:value-of select="$y1" />px;
	      </xsl:attribute>
	    </div>
	  </xsl:if>
	  <!-- ENF OF TEXT REGION -->

	  <!-- PARAGRAPH -->
	  <xsl:if test="name() = 'paragraph'">
	    <div class="para">
	      <xsl:attribute name="style">
		height:<xsl:value-of select="$y2 - $y1" />px;
		width:<xsl:value-of select="$x2 - $x1" />px;
		left:<xsl:value-of select="$x1" />px;
		top:<xsl:value-of select="$y1" />px;
	      </xsl:attribute>
	    </div>
	  </xsl:if>
	  <!-- ENF OF PARAGRAPH -->

	  <!-- NON-TEXT REGIONS -->
	  <xsl:if test="name() = 'image_region' or name() = 'separator_region' or name() = 'graphic_region' or name() = 'chart_region' or name() = 'table_region'">

	    <!-- id -->
	    <xsl:variable name="id">
	      <xsl:value-of select="@id" />
	    </xsl:variable>

	    <!-- depth -->
	    <xsl:variable name="depth">
	      <xsl:choose>
		<xsl:when test="name() = 'separator_region'">
		  1
		</xsl:when>
		<xsl:otherwise>
		  4
		</xsl:otherwise>
	      </xsl:choose>
	    </xsl:variable>

	    <div class="image">
	      <xsl:attribute name="style">
		top:<xsl:value-of select="$y1" />px;
		left:<xsl:value-of select="$x1" />px;
		width:<xsl:value-of select="$x2 - $x1"/>px;
		height:<xsl:value-of select="$y2 - $y1"/>px;
		z-index:<xsl:value-of select="$depth"/>;
	      </xsl:attribute>
	      <img>
		<xsl:attribute name="alt">
		  <xsl:value-of select="name()" />
		</xsl:attribute>
		<xsl:attribute name="width">
		  <xsl:value-of select="$x2 - $x1" />
		</xsl:attribute>
		<xsl:attribute name="height">
		  <xsl:value-of select="$y2 - $y1" />
		</xsl:attribute>
		<xsl:attribute name="src">
		  img/<xsl:value-of select="$id"/>.png</xsl:attribute>
	      </img>
	    </div>

	  </xsl:if>
	  <!-- END NON-TEXT REGIONS -->
	</xsl:for-each>
      </body>
    </html>
  </xsl:template>

</xsl:stylesheet>
