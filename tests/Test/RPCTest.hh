<?hh // strict

namespace Ivyhk\Xml\Test;

use Ivyhjk\Xml\RPC;
use Ivyhjk\Xml\Exception\XmlException;

/**
 * Test XML RPC workflow.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhk\Xml\Test
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license Private license
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class RPCTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test encode method with nested values.
     *
     * @return void
     */
    public function testEncode() : void
    {
        // minify the xml.
        $expected = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="utf-8"?>
            <params>
                <param>
                    <value>
                        <struct>
                            <member>
                                <name>foo</name>
                                <value>
                                    <string>bar</string>
                                </value>
                            </member>
                            <member>
                                <name>bar</name>
                                <value>
                                    <string>baz</string>
                                </value>
                            </member>
                            <member>
                                <name>baz</name>
                                <value>
                                    <struct>
                                        <member>
                                            <name>zzz</name>
                                            <value>
                                                <string>xxx</string>
                                            </value>
                                        </member>
                                        <member>
                                            <name>ccc</name>
                                            <value>
                                                <struct>
                                                    <member>
                                                        <name>vvv</name>
                                                        <value>
                                                            <struct>
                                                                <member>
                                                                    <name>bbb</name>
                                                                    <value>
                                                                        <string>nnn</string>
                                                                    </value>
                                                                </member>
                                                            </struct>
                                                        </value>
                                                    </member>
                                                </struct>
                                            </value>
                                        </member>
                                    </struct>
                                </value>
                            </member>
                        </struct>
                    </value>
                </param>
            </params>');

        $parameters = Map{
            'foo' => 'bar',
            'bar' => 'baz',
            'baz' => Map{
                'zzz' => 'xxx',
                'ccc' => Map{
                    'vvv' => Map{
                        'bbb' => 'nnn'
                    }
                }
            }
        };

        $encoded = preg_replace('/\n/', '', RPC::encode($parameters));

        $this->assertSame($expected, $encoded);
    }

    /**
     * Test the decode method when is sended an invalid xml as input.
     *
     * @return void
     */
    public function testDecodeError() : void
    {
        $this->expectException(XmlException::class);
        RPC::decode('');
    }

    /**
     * Test the decode method workflow with a basic value, like a string.
     *
     * @return void
     */
    public function testDecodeString() : void
    {
        $xml = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="utf-8"?>
            <params>
                <param>
                    <value>
                        <string>foo</string>
                    </value>
                </param>
            </params>'
        );

        $decoded = RPC::decode($xml);

        $this->assertSame('foo', $decoded);
    }

    /**
     * Test the decode method workflow with a basic value, like an integer.
     *
     * @return void
     */
    public function testDecodeInteger() : void
    {
        $xml = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="utf-8"?>
            <params>
                <param>
                    <value>
                        <int>1337</int>
                    </value>
                </param>
            </params>'
        );

        $decoded = RPC::decode($xml);

        $this->assertSame(1337, $decoded);
    }

    /**
     * Test the decode method workflow with a basic value, like a double.
     *
     * @return void
     */
    public function testDecodeDouble() : void
    {
        $xml = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="utf-8"?>
            <params>
                <param>
                    <value>
                        <float>13.37</float>
                    </value>
                </param>
            </params>'
        );

        $decoded = RPC::decode($xml);

        $this->assertSame(13.37, $decoded);
    }

    /**
     * Test the decode method worflow with nested values.
     *
     * @return void
     */
    public function testDecodeNestedValues() : void
    {
        // minify the xml.
        $xml = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="utf-8"?>
            <params>
                <param>
                    <value>
                        <struct>
                            <member>
                                <name>foo</name>
                                <value>
                                    <string>bar</string>
                                </value>
                            </member>
                            <member>
                                <name>bar</name>
                                <value>
                                    <string>baz</string>
                                </value>
                            </member>
                            <member>
                                <name>baz</name>
                                <value>
                                    <struct>
                                        <member>
                                            <name>zzz</name>
                                            <value>
                                                <string>xxx</string>
                                            </value>
                                        </member>
                                        <member>
                                            <name>ccc</name>
                                            <value>
                                                <struct>
                                                    <member>
                                                        <name>vvv</name>
                                                        <value>
                                                            <struct>
                                                                <member>
                                                                    <name>bbb</name>
                                                                    <value>
                                                                        <string>nnn</string>
                                                                    </value>
                                                                </member>
                                                            </struct>
                                                        </value>
                                                    </member>
                                                </struct>
                                            </value>
                                        </member>
                                    </struct>
                                </value>
                            </member>
                        </struct>
                    </value>
                </param>
            </params>'
        );

        $decoded = RPC::decode($xml);

        $expected = Map{
            'foo' => 'bar',
            'bar' => 'baz',
            'baz' => Map{
                'zzz' => 'xxx',
                'ccc' => Map{
                    'vvv' => Map{
                        'bbb' => 'nnn'
                    }
                }
            }
        };

        $this->assertEquals($expected, $decoded);
    }
}
